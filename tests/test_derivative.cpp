#include <gtest/gtest.h>
#include "parser/parser.h"
#include "evaluator/derivative.h"
#include <cmath>
#include <variant>

static double deriv(const std::string& expr, double x) {
    Parser parser(expr);
    ParseResult result = parser.parse();
    const ParsedFunction& fn = std::get<ParsedFunction>(result);
    return NumericalDerivative::compute(*fn.expr, x);
}

TEST(DerivativeTest, LinearFunction) {
    EXPECT_NEAR(deriv("y = 3*x + 2", 5.0), 3.0, 1e-5);
}

TEST(DerivativeTest, Quadratic) {
    EXPECT_NEAR(deriv("y = x^2", 3.0), 6.0, 1e-5);
}

TEST(DerivativeTest, SinIsCos) {
    EXPECT_NEAR(deriv("y = sin(x)", 1.0), std::cos(1.0), 1e-5);
}

TEST(DerivativeTest, ExpIsExp) {
    EXPECT_NEAR(deriv("y = exp(x)", 2.0), std::exp(2.0), 1e-5);
}

TEST(DerivativeTest, ConstantIsZero) {
    EXPECT_NEAR(deriv("y = 42", 5.0), 0.0, 1e-5);
}

TEST(DerivativeTest, NaNForUndefined) {
    double result = deriv("y = sqrt(x)", -1.0);
    EXPECT_TRUE(std::isnan(result));
}

TEST(DerivativeTest, Cubic) {
    EXPECT_NEAR(deriv("y = x^3", 2.0), 12.0, 1e-4);
}

TEST(DerivativeTest, LnDerivative) {
    EXPECT_NEAR(deriv("y = ln(x)", 2.0), 0.5, 1e-5);
}
