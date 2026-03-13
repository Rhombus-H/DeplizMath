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
    const ParsedEquation& eq = std::get<ParsedEquation>(item.parsed);

    Color col = ColorPalette::getColor(colorIdx);
    ImU32 drawColor = IM_COL32(col.r, col.g, col.b, 220);

    // marching squares подход: для каждой ячейки проверяем
    // знак f = lhs - rhs в 4 углах. если знак меняется - рисуем
    int cellSize = 2; // размер ячейки в пикселях
    int w = (int)plotW;
    int h = (int)plotH;

    ImDrawList* drawList = ImPlot::GetPlotDrawList();

    double totalRangeX = viewport->getRangeX();
    double totalRangeY = viewport->getRangeY();

    // dx и dy - размер ячейки в мат координатах
    double dx = (double)cellSize * totalRangeX / plotW;
    double dy = (double)cellSize * totalRangeY / plotH;

    for (int screenY = 0; screenY < h; screenY += cellSize) {
        for (int screenX = 0; screenX < w; screenX += cellSize) {
            // координаты углов ячейки
            double mx = viewport->screenToMathX(screenX, plotW);
            double my = viewport->screenToMathY(screenY, plotH);

            // f = lhs - rhs в четырех углах
            double f00 = Evaluator::evaluate(*eq.lhs, mx, my) - Evaluator::evaluate(*eq.rhs, mx, my);
            double f10 = Evaluator::evaluate(*eq.lhs, mx + dx, my) - Evaluator::evaluate(*eq.rhs, mx + dx, my);
            double f01 = Evaluator::evaluate(*eq.lhs, mx, my - dy) - Evaluator::evaluate(*eq.rhs, mx, my - dy);
            double f11 = Evaluator::evaluate(*eq.lhs, mx + dx, my - dy) - Evaluator::evaluate(*eq.rhs, mx + dx, my - dy);

            // если какой-то NaN - пропускаем
            if (std::isnan(f00) || std::isnan(f10) || std::isnan(f01) || std::isnan(f11)) {
                continue;
            }

            // проверяем: все ли одного знака?
            bool allPositive = (f00 > 0) && (f10 > 0) && (f01 > 0) && (f11 > 0);
            bool allNegative = (f00 < 0) && (f10 < 0) && (f01 < 0) && (f11 < 0);

            if (!allPositive && !allNegative) {
                // знак меняется - кривая проходит через эту ячейку
                ImVec2 pMin = ImPlot::PlotToPixels(mx, my);
                ImVec2 pMax = ImPlot::PlotToPixels(mx + dx, my - dy);
                drawList->AddRectFilled(pMin, pMax, drawColor);
            }
        }
    }
}

void Plotter::drawInequality(const PlotItem& item, std::size_t colorIdx,
                              float plotW, float plotH) {
    const ParsedInequality& ineq = std::get<ParsedInequality>(item.parsed);

    Color fillCol = ColorPalette::withAlpha(colorIdx, 60);
    ImU32 fillColor = IM_COL32(fillCol.r, fillCol.g, fillCol.b, fillCol.a);

    int stepSize = 4;
    int w = (int)plotW;
    int h = (int)plotH;

    ImDrawList* drawList = ImPlot::GetPlotDrawList();

    double totalRangeX = viewport->getRangeX();
    double totalRangeY = viewport->getRangeY();

    for (int screenY = 0; screenY < h; screenY += stepSize) {
        for (int screenX = 0; screenX < w; screenX += stepSize) {
            double mathX = viewport->screenToMathX(screenX + stepSize / 2.0, plotW);
            double mathY = viewport->screenToMathY(screenY + stepSize / 2.0, plotH);

            bool sat = Evaluator::satisfies(ineq, mathX, mathY);
            if (sat) {
                double halfW = (double)stepSize * totalRangeX / plotW / 2.0;
                double halfH = (double)stepSize * totalRangeY / plotH / 2.0;

                ImVec2 pMin = ImPlot::PlotToPixels(mathX - halfW, mathY + halfH);
                ImVec2 pMax = ImPlot::PlotToPixels(mathX + halfW, mathY - halfH);
                drawList->AddRectFilled(pMin, pMax, fillColor);
            }
        }
    }

    // граничная линия для простых неравенств (y > f(x))
    if (!ineq.twoVariable) {
        int numPoints = (int)(plotW) * 2;
        if (numPoints < 100) numPoints = 100;

        double xMin = viewport->getXMin();
        double xMax = viewport->getXMax();
        double rangeY = viewport->getRangeY();
        double xStep = (xMax - xMin) / (double)(numPoints - 1);

        // сегменты
        std::vector<std::vector<double>> segXs;
        std::vector<std::vector<double>> segYs;
        std::vector<double> curX;
        std::vector<double> curY;
        double prevY = std::numeric_limits<double>::quiet_NaN();

        for (int i = 0; i < numPoints; i++) {
            double x = xMin + i * xStep;
            double y = Evaluator::evaluate(*ineq.rhs, x);

            if (std::isnan(y) || std::isinf(y)) {
                if (curX.size() > 0) {
                    segXs.push_back(curX);
                    segYs.push_back(curY);
                    curX.clear();
                    curY.clear();
                }
                prevY = std::numeric_limits<double>::quiet_NaN();
                continue;
            }

            if (Evaluator::isDiscontinuity(prevY, y, rangeY)) {
                if (curX.size() > 0) {
                    segXs.push_back(curX);
                    segYs.push_back(curY);
                    curX.clear();
                    curY.clear();
                }
            }

            curX.push_back(x);
            curY.push_back(y);
            prevY = y;
        }

        if (curX.size() > 0) {
            segXs.push_back(curX);
            segYs.push_back(curY);
        }

        Color lineCol = ColorPalette::getColor(colorIdx);
        ImVec4 lineColor(lineCol.r / 255.0f, lineCol.g / 255.0f,
                         lineCol.b / 255.0f, 1.0f);

        for (std::size_t s = 0; s < segXs.size(); s++) {
            std::string label = "##boundary_" + std::to_string(s) + "_" + item.expression;

            float thickness = 2.0f;
            if (ineq.op == CompareOp::Greater || ineq.op == CompareOp::Less) {
                thickness = 1.5f;
            }

            ImPlot::SetNextLineStyle(lineColor, thickness);
            ImPlot::PlotLine(label.c_str(),
                             segXs[s].data(), segYs[s].data(),
                             (int)segXs[s].size());
        }
    }
}
