#pragma once

#include "core/ast.h"

// num-derivative
class NumericalDerivative {
public:
    static constexpr double H = 1e-8; // step value

    // (f(x+h) - f(x-h)) / 2h - meaning, the definition of derivative
    static double compute(const ExprNode& expr, double x);
};
