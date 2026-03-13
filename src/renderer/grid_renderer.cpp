#include "renderer/grid_renderer.h"

GridRenderer::GridRenderer(std::shared_ptr<Viewport> vp)
    : viewport(std::move(vp)) {}

void GridRenderer::configure() {
    // задаем имена осей и лимиты
    ImPlot::SetupAxes("x", "y");

    double xMin = viewport->getXMin();
    double xMax = viewport->getXMax();
    double yMin = viewport->getYMin();
    double yMax = viewport->getYMax();
}
