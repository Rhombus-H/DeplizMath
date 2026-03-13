#pragma once

#include <memory>
#include "renderer/viewport.h"

class GridRenderer {
public:
    explicit GridRenderer(std::shared_ptr<Viewport> vp);
    void configure(); // настраивает оси ImPlot перед рисованием

private:
    std::shared_ptr<Viewport> viewport;
};
