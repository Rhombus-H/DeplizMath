#include <cmath>
#include <limits>
#include <string>
#include <variant>
#include "evaluator/evaluator.h"
#include "core/constants.h"

// computing build-in functions in C++ STD
double Evaluator::callBuiltinFunction(const std::string& name, double arg) {
    // sin cos tan
    if (name == "sin") {
        return std::sin(arg);
    }
    if (name == "cos") {
        return std::cos(arg);
    }
    if (name == "tan") {
        return std::tan(arg);
    }

    // inverse trigonometric
    if (name == "asin") {
        // asin is defined only on [-1, 1]
        if (arg < -1.0) return std::numeric_limits<double>::quiet_NaN();
        if (arg > 1.0) return std::numeric_limits<double>::quiet_NaN();
        return std::asin(arg);
    }
    if (name == "acos") {
        if (arg < -1.0) return std::numeric_limits<double>::quiet_NaN();
        if (arg > 1.0) return std::numeric_limits<double>::quiet_NaN();
        return std::acos(arg);
    }
    if (name == "atan") {
        return std::atan(arg);
    }

    // logarithms
    if (name == "log") {
        if (arg <= 0.0) return std::numeric_limits<double>::quiet_NaN();
        double result = std::log10(arg);
        return result;
    }
    if (name == "ln") {
        if (arg <= 0.0) return std::numeric_limits<double>::quiet_NaN();
        double result = std::log(arg);
        return result;
    }

    // root
    if (name == "sqrt") {
        if (arg < 0.0) {
            return std::numeric_limits<double>::quiet_NaN();
        }
        return std::sqrt(arg);
    }

    if (name == "abs") return std::abs(arg);
    if (name == "exp") return std::exp(arg);
    if (name == "ceil") return std::ceil(arg);
    if (name == "floor") return std::floor(arg);

    if (name == "sign") {
        if (arg > 0.0) return 1.0;
        if (arg < 0.0) return -1.0;
        return 0.0;
    }

    // if the function is not found (just in case)
    return std::numeric_limits<double>::quiet_NaN();
}

double Evaluator::evaluate(const ExprNode& node, double x, double y) {
    // we use std::visit for passing the variant
    double result = std::visit([&](const auto& data) -> double {
        using T = std::decay_t<decltype(data)>;

        if constexpr (std::is_same_v<T, NumberLit>) {
            return data.value;

        } else if constexpr (std::is_same_v<T, VariableLit>) {
            if (data.name == "x") return x;
            if (data.name == "y") return y;
            // unknown variable
            return std::numeric_limits<double>::quiet_NaN();

        } else if constexpr (std::is_same_v<T, BinaryOp>) {
            double leftVal = evaluate(*data.left, x, y);
            double rightVal = evaluate(*data.right, x, y);

            if (data.op == '+') return leftVal + rightVal;
            if (data.op == '-') return leftVal - rightVal;
            if (data.op == '*') return leftVal * rightVal;

            if (data.op == '/') {
                // division by zero -> NaN
                if (rightVal == 0.0) {
                    return std::numeric_limits<double>::quiet_NaN();
                }
                return leftVal / rightVal;
            }

            if (data.op == '^') {
                double res = std::pow(leftVal, rightVal);
                return res;
            }

            return std::numeric_limits<double>::quiet_NaN();

        } else if constexpr (std::is_same_v<T, UnaryOp>) {
            double val = evaluate(*data.operand, x, y);
            if (data.op == '-') {
                return -val;
            }
            return val;

        } else if constexpr (std::is_same_v<T, FuncCall>) {
            if (data.args.size() == 0) {
                return std::numeric_limits<double>::quiet_NaN();
            }
            double argVal = evaluate(*data.args[0], x, y);
            double funcResult = callBuiltinFunction(data.name, argVal);
            return funcResult;

        } else {
            return std::numeric_limits<double>::quiet_NaN();
        }
    }, node.data);

    return result;
}

bool Evaluator::satisfies(const ParsedInequality& ineq, double x, double y) {
    double leftValue = evaluate(*ineq.lhs, x, y);
    double rightValue = evaluate(*ineq.rhs, x, y);

    // if NaN then it does not satisfy
    bool leftNan = std::isnan(leftValue);
    bool rightNan = std::isnan(rightValue);
    if (leftNan || rightNan) return false;

    // checking operator
    if (ineq.op == CompareOp::Greater) {
        return leftValue > rightValue;
    } else if (ineq.op == CompareOp::Less) {
        return leftValue < rightValue;
    } else if (ineq.op == CompareOp::GreaterEqual) {
        return leftValue >= rightValue;
    } else if (ineq.op == CompareOp::LessEqual) {
        return leftValue <= rightValue;
    }

    return false; // should not happen, just in case
}