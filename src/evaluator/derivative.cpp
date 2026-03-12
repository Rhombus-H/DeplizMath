#include "evaluator/derivative.h"
#include "evaluator/evaluator.h"
#include <cmath>
#include <limits>

double NumericalDerivative::compute(const ExprNode& expr, double x) {
    double h = H; // using a constant from the class

    double f_plus = Evaluator::evaluate(expr, x + h);
    double f_minus = Evaluator::evaluate(expr, x - h);

    // well-defined values check
    if (std::isnan(f_plus) || std::isnan(f_minus)) {
        return std::numeric_limits<double>::quiet_NaN();
    }
    if (std::isinf(f_plus) || std::isinf(f_minus)) {
        return std::numeric_limits<double>::quiet_NaN();
    }

    // main difference
    double derivative = (f_plus - f_minus) / (2.0 * h);
    return derivative;
}
