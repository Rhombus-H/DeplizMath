#include "renderer/plotter.h"
#include "evaluator/evaluator.h"
#include "core/color_palette.h"
#include <cmath>
#include <vector>
#include <variant>
#include <limits>
#include <string>


#include "implot.h"

Plotter::Plotter(std::shared_ptr<Viewport> vp)
    : viewport(std::move(vp)) {}

void Plotter::render(const Pipeline& pipeline, float plotW, float plotH) {
    for (std::size_t i = 0; i < pipeline.items().size(); i++) {
        const PlotItem& item = *pipeline.items()[i];
        if (!item.visible) continue;

        // определяем тип через visit
        std::visit([&](const auto& parsed) {
            using T = std::decay_t<decltype(parsed)>;
            if constexpr (std::is_same_v<T, ParsedFunction>) {
                drawFunction(item, item.color_index, plotW);
            } else if constexpr (std::is_same_v<T, ParsedEquation>) {
                drawEquation(item, item.color_index, plotW, plotH);
            } else if constexpr (std::is_same_v<T, ParsedInequality>) {
                drawInequality(item, item.color_index, plotW, plotH);
            }
        }, item.parsed);
    }
}

void Plotter::drawFunction(const PlotItem& item, std::size_t colorIdx, float plotW) {
    const ParsedFunction& func = std::get<ParsedFunction>(item.parsed);

    int numPoints = (int)(plotW) * 2;
    if (numPoints < 100) numPoints = 100;

    double xMin = viewport->getXMin();
    double xMax = viewport->getXMax();
    double rangeY = viewport->getRangeY();
    double step = (xMax - xMin) / (double)(numPoints - 1);

    // сегменты с разрывами
    std::vector<std::vector<double>> allSegX;
    std::vector<std::vector<double>> allSegY;
    std::vector<double> currentSegX;
    std::vector<double> currentSegY;
    double prevY = std::numeric_limits<double>::quiet_NaN();

    for (int i = 0; i < numPoints; i++) {
        double x = xMin + i * step;
        double y = Evaluator::evaluate(*func.expr, x);

        if (std::isnan(y) || std::isinf(y)) {
            if (currentSegX.size() > 0) {
                allSegX.push_back(currentSegX);
                allSegY.push_back(currentSegY);
                currentSegX.clear();
                currentSegY.clear();
            }
            prevY = std::numeric_limits<double>::quiet_NaN();
            continue;
        }

        if (Evaluator::isDiscontinuity(prevY, y, rangeY)) {
            if (currentSegX.size() > 0) {
                allSegX.push_back(currentSegX);
                allSegY.push_back(currentSegY);
                currentSegX.clear();
                currentSegY.clear();
            }
        }

        currentSegX.push_back(x);
        currentSegY.push_back(y);
        prevY = y;
    }

    if (currentSegX.size() > 0) {
        allSegX.push_back(currentSegX);
        allSegY.push_back(currentSegY);
    }

    Color col = ColorPalette::getColor(colorIdx);
    ImVec4 lineColor(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 1.0f);

    for (std::size_t s = 0; s < allSegX.size(); s++) {
        std::string label;
        if (s == 0) {
            label = item.expression;
        } else {
            label = "##seg" + std::to_string(s) + "_" + item.expression;
        }

        ImPlot::SetNextLineStyle(lineColor, 2.0f);
        ImPlot::PlotLine(label.c_str(),
                         allSegX[s].data(), allSegY[s].data(),
                         (int)allSegX[s].size());
    }
}

void Plotter::drawEquation(const PlotItem& item, std::size_t colorIdx,
                            float plotW, float plotH) {
}

void Plotter::drawInequality(const PlotItem& item, std::size_t colorIdx,
                              float plotW, float plotH) {
}
