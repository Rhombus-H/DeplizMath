add_library(deplizmath_evaluator STATIC
        evaluator.cpp
        derivative.cpp
)

target_include_directories(deplizmath_evaluator PUBLIC ${PROJECT_SOURCE_DIR}/include)

# Evaluator depends on parser (for AST types)
target_link_libraries(deplizmath_evaluator PUBLIC deplizmath_parser)
