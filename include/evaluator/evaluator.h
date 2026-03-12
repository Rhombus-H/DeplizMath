#pragma once

#include <string>
#include "core/ast.h"

class Evaluator {
public:
    // computes value at (x,y)
    static double evaluate(const ExprNode& node, double x, double y = 0.0);

    // checks whether point satisfies inequality
    static bool satisfies(const ParsedInequality& ineq, double x, double y);

    // determines points of discontinuity
    static bool isDiscontinuity(double y_prev, double y_curr, double rangeY);

    // defines suitable, i.e. "nice" step
    static double niceStep(double range);

    // checks whether the expr contains variable (x or y)
    static bool containsVar(const ExprNode& node, const std::string& varName);

private:
    static double callBuiltinFunction(const std::string& name, double arg);
};
