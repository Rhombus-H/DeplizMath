#include "app/app.h"
#include "app/exporter.h"
#include "core/errors.h"
#include "core/color_palette.h"

#include <SFML/Window/Event.hpp>
#include <SFML/System/Clock.hpp>

#include "imgui.h"
#include "imgui-SFML.h"
#include "implot.h"

#include <cstring>

App::App()
    : viewport(std::make_shared<Viewport>()),
      plotter(std::make_unique<Plotter>(viewport)),
      gridRenderer(std::make_unique<GridRenderer>(viewport)),
      window(sf::VideoMode(1280, 720), "MathPlot", sf::Style::Default) {

    // очищаем буфер ввода
    inputBuf.fill('\0');

    // начальные значения viewport для ui полей
    vpInputs[0] = (float)viewport->getXMin();
    vpInputs[1] = (float)viewport->getXMax();
    vpInputs[2] = (float)viewport->getYMin();
    vpInputs[3] = (float)viewport->getYMax();

    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);
    ImPlot::CreateContext();
}

App::~App() {
    ImPlot::DestroyContext();
    ImGui::SFML::Shutdown();
}

void App::run() {
    sf::Clock deltaClock;

    while (isRunning && window.isOpen()) {
        handleEvents();

        sf::Time dt = deltaClock.restart();
        ImGui::SFML::Update(window, dt);

        drawUI();

        window.clear(sf::Color(30, 30, 30));
        ImGui::SFML::Render(window);
        window.display();
    }
}

void App::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(window, event);

        if (event.type == sf::Event::Closed) {
            isRunning = false;
            window.close();
        }
    }
}

void App::updateViewport() {
    viewport->setXMin((double)vpInputs[0]);
    viewport->setXMax((double)vpInputs[1]);
    viewport->setYMin((double)vpInputs[2]);
    viewport->setYMax((double)vpInputs[3]);
}

void App::drawUI() {
    drawControlPanel();
    drawPlotWindow();
}

void App::drawControlPanel() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(320, 720), ImGuiCond_FirstUseEver);
    ImGui::Begin("Control Panel");

    // --- ввод выражения ---
    ImGui::Text("Expression:");
    bool pressedEnter = ImGui::InputText("##expr", inputBuf.data(), inputBuf.size(),
                                          ImGuiInputTextFlags_EnterReturnsTrue);
    ImGui::SameLine();
    bool clickedAdd = ImGui::Button("Add");

    if (pressedEnter || clickedAdd) {
        std::string expr(inputBuf.data());
        if (expr.size() > 0) {
            saveSnapshot();
            bool success = pipeline.add(expr);
            if (success) {
                statusMsg = "Added: " + expr;
                inputBuf.fill('\0');
            } else {
                statusMsg = "Error: Invalid expression";
            }
        }
    }

    // status message
    if (statusMsg.size() > 0) {
        ImGui::TextColored(ImVec4(0.8f, 0.8f, 0.2f, 1.0f), "%s", statusMsg.c_str());
    }

    ImGui::Separator();

    // --- список функций ---
    std::size_t count = pipeline.size();
    ImGui::Text("Functions (%d):", (int)count);

    int removeIndex = -1;

    for (std::size_t i = 0; i < pipeline.items().size(); i++) {
        const PlotItem& item = *pipeline.items()[i];
        Color col = ColorPalette::getColor(item.color_index);

        ImGui::PushID((int)i);

        // квадратик с цветом
        ImVec4 imCol(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);
        ImGui::ColorButton("##color", imCol, ImGuiColorEditFlags_NoTooltip, ImVec2(14, 14));
        ImGui::SameLine();

        // checkbox видимости
        bool vis = item.visible;
        if (ImGui::Checkbox("##vis", &vis)) {
            pipeline.toggle(i);
        }
        ImGui::SameLine();

        // текст выражения
        ImGui::TextWrapped("%s", item.expression.c_str());

        // кнопка удаления
        ImGui::SameLine();
        if (ImGui::SmallButton("X")) {
            removeIndex = (int)i;
        }

        ImGui::PopID();
    }

    // удаляем если нажали X
    if (removeIndex >= 0) {
        saveSnapshot();
        pipeline.remove((std::size_t)removeIndex);
    }

    ImGui::Separator();

    // --- undo redo ---
    if (ImGui::Button("Undo")) {
        if (history.canUndo()) {
            std::optional<std::vector<std::string>> prev = history.undo();
            if (prev.has_value()) {
                std::vector<std::string> exprs = prev.value();
                pipeline.restoreFromExpressions(exprs);
                statusMsg = "Undo";
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Redo")) {
        if (history.canRedo()) {
            std::optional<std::vector<std::string>> next = history.redo();
            if (next.has_value()) {
                std::vector<std::string> exprs = next.value();
                pipeline.restoreFromExpressions(exprs);
                statusMsg = "Redo";
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear All")) {
        saveSnapshot();
        pipeline.clear();
        statusMsg = "Cleared";
    }

    ImGui::Separator();

    // --- viewport input ---
    ImGui::Text("Viewport:");

    bool changed = false;
    if (ImGui::InputFloat("X Min", &vpInputs[0], 1.0f, 5.0f, "%.2f")) changed = true;
    if (ImGui::InputFloat("X Max", &vpInputs[1], 1.0f, 5.0f, "%.2f")) changed = true;
    if (ImGui::InputFloat("Y Min", &vpInputs[2], 1.0f, 5.0f, "%.2f")) changed = true;
    if (ImGui::InputFloat("Y Max", &vpInputs[3], 1.0f, 5.0f, "%.2f")) changed = true;

    if (changed) {
        updateViewport();
        viewportDirty = true; // сообщаем ImPlot что лимиты изменились
    }

    if (ImGui::Button("Reset View")) {
        viewport->reset();
        // обновляем поля
        vpInputs[0] = (float)viewport->getXMin();
        vpInputs[1] = (float)viewport->getXMax();
        vpInputs[2] = (float)viewport->getYMin();
        vpInputs[3] = (float)viewport->getYMax();
        viewportDirty = true; // принудительно обновить ImPlot
    }

    ImGui::Separator();

    // --- export ---
    if (ImGui::Button("Export PNG")) {
        std::optional<std::string> err = Exporter::save(window, "mathplot_export.png");
        if (err.has_value()) {
            statusMsg = err.value();
        } else {
            statusMsg = "Exported to mathplot_export.png";
        }
    }

    ImGui::End();
}

void App::drawPlotWindow() {
    ImGui::SetNextWindowPos(ImVec2(320, 0), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(960, 720), ImGuiCond_FirstUseEver);
    ImGui::Begin("Plot", nullptr, ImGuiWindowFlags_NoScrollbar);

    ImVec2 avail = ImGui::GetContentRegionAvail();
    float plotW = avail.x;
    float plotH = avail.y;

    if (ImPlot::BeginPlot("##MainPlot", ImVec2(plotW, plotH), ImPlotFlags_Equal)) {
        // только принудительно ставим лимиты когда пользователь их изменил вручную
        // иначе ImPlot сам рулит зумом и паном
        gridRenderer->configure(viewportDirty);
        viewportDirty = false;

        // читаем обратно лимиты из ImPlot
        // (пользователь мог зумить колесиком или панить мышкой)
        ImPlotRect limits = ImPlot::GetPlotLimits();

        viewport->setXMin(limits.X.Min);
        viewport->setXMax(limits.X.Max);
        viewport->setYMin(limits.Y.Min);
        viewport->setYMax(limits.Y.Max);

        // sync ui inputs
        vpInputs[0] = (float)limits.X.Min;
        vpInputs[1] = (float)limits.X.Max;
        vpInputs[2] = (float)limits.Y.Min;
        vpInputs[3] = (float)limits.Y.Max;

        // рисуем графики
        plotter->render(pipeline, plotW, plotH);

        ImPlot::EndPlot();
    }

    ImGui::End();
}

void App::saveSnapshot() {
    std::vector<std::string> currentExprs = pipeline.getExpressions();
    history.push(currentExprs);
}
