#pragma once

#include <memory>
#include "renderer/viewport.h"

class GridRenderer {
public:
    explicit GridRenderer(std::shared_ptr<Viewport> vp);

    // forceLimits = true когда пользователь вручную изменил viewport
    void configure(bool forceLimits);

private:
    std::shared_ptr<Viewport> viewport;
};

