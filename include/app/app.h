#pragma once

#include <memory>
#include <string>
#include <array>
#include <SFML/Graphics/RenderWindow.hpp>
#include "renderer/viewport.h"
#include "renderer/plotter.h"
#include "renderer/grid_renderer.h"
#include "app/pipeline.h"
#include "app/history.h"

class App {
public:
    App();
    ~App();

    void run(); // основной цикл

private:
    // shared_ptr потому что viewport используется в нескольких местах
    std::shared_ptr<Viewport> viewport;
    std::unique_ptr<Plotter> plotter;
    std::unique_ptr<GridRenderer> gridRenderer;
    Pipeline pipeline;
    History history;

    sf::RenderWindow window;
    bool isRunning = true;

    // ui stuff
    std::array<char, 256> inputBuf;
    std::string statusMsg;
    float vpInputs[4]; // xmin, xmax, ymin, ymax
    bool viewportDirty = true; // true = нужно принудительно обновить лимиты ImPlot

    void handleEvents();
    void updateViewport();
    void drawUI();
    void drawControlPanel();
    void drawPlotWindow();
    void saveSnapshot();
};
