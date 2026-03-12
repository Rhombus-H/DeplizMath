add_library(mathplot_parser STATIC
        tokenizer.cpp
        parser.cpp
)

target_include_directories(mathplot_parser PUBLIC ${PROJECT_SOURCE_DIR}/include)
