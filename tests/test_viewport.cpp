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

// checks if math -> screen -> math is valid
TEST(ViewportTest, RoundTrip) {
    Viewport v(-10, 10, -10, 10);
    double sw = 800.0;
    double sh = 600.0;
    double mathX = 3.5;
    double mathY = -2.7;
    double screenX = v.mathToScreenX(mathX, sw);
    double screenY = v.mathToScreenY(mathY, sh);
    double backX = v.screenToMathX(screenX, sw);
    double backY = v.screenToMathY(screenY, sh);
    EXPECT_NEAR(backX, mathX, 1e-10);
    EXPECT_NEAR(backY, mathY, 1e-10);
}

// checks if zoomIn is working correctly
TEST(ViewportTest, ZoomIn) {
    Viewport v(-10, 10, -10, 10);
    v.zoom(0.5, 0.0, 0.0);
    EXPECT_NEAR(v.getXMin(), -5.0, 1e-10);
    EXPECT_NEAR(v.getXMax(),  5.0, 1e-10);
    EXPECT_NEAR(v.getYMin(), -5.0, 1e-10);
    EXPECT_NEAR(v.getYMax(),  5.0, 1e-10);
}

// checks if zoomOut is working correctly
TEST(ViewportTest, ZoomOut) {
    Viewport v(-10, 10, -10, 10);
    v.zoom(2.0, 0.0, 0.0);
    EXPECT_NEAR(v.getXMin(), -20.0, 1e-10);
    EXPECT_NEAR(v.getXMax(),  20.0, 1e-10);
}

TEST(ViewportTest, Pan) {
    Viewport v(-10, 10, -10, 10);
    v.pan(5.0, 3.0);
    EXPECT_NEAR(v.getXMin(), -5.0, 1e-10);
    EXPECT_NEAR(v.getXMax(), 15.0, 1e-10);
    EXPECT_NEAR(v.getYMin(), -7.0, 1e-10);
    EXPECT_NEAR(v.getYMax(), 13.0, 1e-10);
}

TEST(ViewportTest, Reset) {
    Viewport v(-10, 10, -10, 10);
    v.zoom(0.5, 0.0, 0.0);
    v.pan(5.0, 3.0);
    v.reset();
    EXPECT_DOUBLE_EQ(v.getXMin(), -10.0);
    EXPECT_DOUBLE_EQ(v.getXMax(),  10.0);
}

TEST(ViewportTest, GridStep) {
    Viewport v(-10, 10, -10, 10);
    double stepX = v.gridStepX();
    EXPECT_TRUE(stepX > 0);
    EXPECT_TRUE(stepX == 1.0 || stepX == 2.0 || stepX == 5.0);
}

TEST(ViewportTest, RangeCalculation) {
    Viewport v(-5, 15, -3, 7);
    EXPECT_DOUBLE_EQ(v.getRangeX(), 20.0);
    EXPECT_DOUBLE_EQ(v.getRangeY(), 10.0);
}
