#pragma once

// класс для хранения координат и преобразований
class Viewport {
public:
    Viewport();
    Viewport(double xMin, double xMax, double yMin, double yMax);

    // convert screen coordinates to math and back
    double screenToMathX(double screenX, double screenWidth) const;
    double screenToMathY(double screenY, double screenHeight) const;
    double mathToScreenX(double mathX, double screenWidth) const;
    double mathToScreenY(double mathY, double screenHeight) const;

    void zoom(double factor, double cx, double cy);
    void pan(double dx, double dy);
    void reset();

    // getters
    double getXMin() const { return xMin; }
    double getXMax() const { return xMax; }
    double getYMin() const { return yMin; }
    double getYMax() const { return yMax; }
    double getRangeX() const { return xMax - xMin; }
    double getRangeY() const { return yMax - yMin; }

    // setters для imgui полей ввода
    void setXMin(double v) { xMin = v; }
    void setXMax(double v) { xMax = v; }
    void setYMin(double v) { yMin = v; }
    void setYMax(double v) { yMax = v; }

    double gridStepX() const;
    double gridStepY() const;

private:
    double xMin, xMax, yMin, yMax;
    // defaults for reset
    double default_xMin, default_xMax, default_yMin, default_yMax;
};
