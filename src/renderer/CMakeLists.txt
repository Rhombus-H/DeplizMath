add_library(deplizmath_renderer STATIC
    viewport.cpp
    plotter.cpp
    grid_renderer.cpp
)

target_include_directories(deplizmath_renderer PUBLIC ${PROJECT_SOURCE_DIR}/include)

# SFML_STATIC is required when linking SFML as a static library
target_compile_definitions(deplizmath_renderer PUBLIC SFML_STATIC)

target_link_libraries(deplizmath_renderer PUBLIC
    deplizmath_evaluator
    sfml-graphics
    sfml-window
    sfml-system
    ImGui-SFML::ImGui-SFML
    implot_lib
)
