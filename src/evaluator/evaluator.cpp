#include <cmath>
#include <limits>
#include <string>

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