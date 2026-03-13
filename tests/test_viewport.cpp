#include <gtest/gtest.h>
#include "renderer/viewport.h"

// checks if default boundries are correct
TEST(ViewportTest, DefaultBounds) {
    Viewport v;
    EXPECT_DOUBLE_EQ(v.getXMin(), -10.0);
    EXPECT_DOUBLE_EQ(v.getXMax(),  10.0);
    EXPECT_DOUBLE_EQ(v.getYMin(), -10.0);
    EXPECT_DOUBLE_EQ(v.getYMax(),  10.0);
}
// checks if setting custom boundries are correct
TEST(ViewportTest, CustomBounds) {
    Viewport v(-5.0, 5.0, -2.0, 8.0);
    EXPECT_DOUBLE_EQ(v.getXMin(), -5.0);
    EXPECT_DOUBLE_EQ(v.getXMax(),  5.0);
    EXPECT_DOUBLE_EQ(v.getYMin(), -2.0);
    EXPECT_DOUBLE_EQ(v.getYMax(),  8.0);
}
// checks if default boundries are correct
TEST(ViewportTest, OriginMapsToCenter) {
    Viewport v(-10, 10, -10, 10);
    double sw = 800.0;
    double sh = 600.0;
    double sx = v.mathToScreenX(0.0, sw);
    double sy = v.mathToScreenY(0.0, sh);
    EXPECT_NEAR(sx, 400.0, 1e-10);
    EXPECT_NEAR(sy, 300.0, 1e-10);
}