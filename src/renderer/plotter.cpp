#include "renderer/plotter.h"
#include "evaluator/evaluator.h"
#include <cmath>
#include <string>

#include "implot.h"

Plotter::Plotter(std::shared_ptr<Viewport> vp)
    : viewport(std::move(vp)) {}

void Plotter::render(const Pipeline& pipeline, float plotW, float plotH) {
}

void Plotter::drawFunction(const PlotItem& item, std::size_t colorIdx, float plotW) {
}

void Plotter::drawEquation(const PlotItem& item, std::size_t colorIdx,
                            float plotW, float plotH) {
}

void Plotter::drawInequality(const PlotItem& item, std::size_t colorIdx,
                              float plotW, float plotH) {
}
