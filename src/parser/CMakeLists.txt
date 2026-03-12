add_library(deplizmath_parser STATIC
        tokenizer.cpp
        parser.cpp
)

target_include_directories(deplizmath_parser PUBLIC ${PROJECT_SOURCE_DIR}/include)
