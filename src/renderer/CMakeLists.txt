add_library(mathplot_renderer STATIC
    viewport.cpp
    plotter.cpp
    grid_renderer.cpp
)

target_include_directories(mathplot_renderer PUBLIC ${PROJECT_SOURCE_DIR}/include)

# SFML_STATIC is required when linking SFML as a static library
target_compile_definitions(mathplot_renderer PUBLIC SFML_STATIC)

target_link_libraries(mathplot_renderer PUBLIC
    mathplot_evaluator
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
    implot_lib
)
