enable_testing()

add_executable(DeplizMathTests
)

target_link_libraries(DeplizMathTests PRIVATE
        deplizmath_lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(DeplizMathTests)
