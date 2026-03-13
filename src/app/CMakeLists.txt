add_library(deplizmath_app STATIC
        pipeline.cpp
        app.cpp
        exporter.cpp
        history.cpp
)

target_include_directories(deplizmath_app PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(deplizmath_app PUBLIC
        deplizmath_renderer
        deplizmath_parser
        sfml-graphics
        sfml-window
        sfml-system
        ImGui-SFML::ImGui-SFML
        implot_lib
)
