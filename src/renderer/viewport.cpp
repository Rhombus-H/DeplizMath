#include "renderer/viewport.h"
#include "evaluator/evaluator.h"

Viewport::Viewport()
    : xMin(-10.0), xMax(10.0), yMin(-10.0), yMax(10.0),
      default_xMin(-10.0), default_xMax(10.0), default_yMin(-10.0), default_yMax(10.0) {}

Viewport::Viewport(double xMin_, double xMax_, double yMin_, double yMax_)
    : xMin(xMin_), xMax(xMax_), yMin(yMin_), yMax(yMax_),
      default_xMin(xMin_), default_xMax(xMax_), default_yMin(yMin_), default_yMax(yMax_) {}

double Viewport::screenToMathX(double screenX, double screenWidth) const {
    // пропорция от 0 до 1
    double ratio = screenX / screenWidth;
    double mathX = xMin + ratio * (xMax - xMin);
    return mathX;
}

double Viewport::screenToMathY(double screenY, double screenHeight) const {
    // экранные координаты сверху вниз, математические снизу вверх
    double ratio = screenY / screenHeight;
    double mathY = yMax - ratio * (yMax - yMin);
    return mathY;
}

double Viewport::mathToScreenX(double mathX, double screenWidth) const {
    double totalRange = xMax - xMin;
    double offset = mathX - xMin;
    double screenX = (offset / totalRange) * screenWidth;
    return screenX;
}

double Viewport::mathToScreenY(double mathY, double screenHeight) const {
    double totalRange = yMax - yMin;
    double offset = yMax - mathY;
    double screenY = (offset / totalRange) * screenHeight;
    return screenY;
}

void Viewport::zoom(double factor, double cx, double cy) {
    double rangeX = xMax - xMin;
    double rangeY = yMax - yMin;

    double newRangeX = rangeX * factor;
    double newRangeY = rangeY * factor;

    double halfX = newRangeX / 2.0;
    double halfY = newRangeY / 2.0;

    xMin = cx - halfX;
    xMax = cx + halfX;
    yMin = cy - halfY;
    yMax = cy + halfY;
}

void Viewport::pan(double dx, double dy) {
    xMin = xMin + dx;
    xMax = xMax + dx;
    yMin = yMin + dy;
    yMax = yMax + dy;
}

void Viewport::reset() {
    xMin = default_xMin;
    xMax = default_xMax;
    yMin = default_yMin;
    yMax = default_yMax;
}

double Viewport::gridStepX() const {
    double range = xMax - xMin;
    double step = Evaluator::niceStep(range);
    return step;
}

double Viewport::gridStepY() const {
    double range = yMax - yMin;
    double step = Evaluator::niceStep(range);
    return step;
}

