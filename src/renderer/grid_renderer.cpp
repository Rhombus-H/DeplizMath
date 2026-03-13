#include "renderer/grid_renderer.h"
#include "implot.h"

GridRenderer::GridRenderer(std::shared_ptr<Viewport> vp)
    : viewport(std::move(vp)) {}

void GridRenderer::configure(bool forceLimits) {
    ImPlot::SetupAxes("x", "y");

    if (forceLimits) {
        // пользователь вручную изменил лимиты или нажал reset
        // принудительно ставим наши значения
        double xMin = viewport->getXMin();
        double xMax = viewport->getXMax();
        double yMin = viewport->getYMin();
        double yMax = viewport->getYMax();

        ImPlot::SetupAxisLimits(ImAxis_X1, xMin, xMax, ImPlotCond_Always);
        ImPlot::SetupAxisLimits(ImAxis_Y1, yMin, yMax, ImPlotCond_Always);
    }
    // если forceLimits == false, не трогаем лимиты
}
