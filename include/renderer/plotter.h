#pragma once

#include <memory>
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

private:
    std::shared_ptr<Viewport> viewport;

};
