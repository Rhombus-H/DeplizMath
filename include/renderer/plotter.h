#pragma once

#include <memory>

#include "app/pipeline.h"
#include "renderer/viewport.h"

// рисует графики через ImPlot
class Plotter {
public:
    explicit Plotter(std::shared_ptr<Viewport> vp);

    Plotter(const Plotter&) = delete;
    Plotter& operator=(const Plotter&) = delete;
    Plotter(Plotter&&) noexcept = default;
    Plotter& operator=(Plotter&&) noexcept = default;
    ~Plotter() = default;

    void render(const Pipeline& pipeline, float plotW, float plotH);
private:
    std::shared_ptr<Viewport> viewport;

    void drawFunction(const PlotItem& item, std::size_t colorIdx, float plotW);
    void drawEquation(const PlotItem& item, std::size_t colorIdx, float plotW, float plotH);
    void drawInequality(const PlotItem& item, std::size_t colorIdx, float plotW, float plotH);

};
