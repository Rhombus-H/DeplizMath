#include <gtest/gtest.h>
#include "parser/parser.h"
#include "evaluator/evaluator.h"
#include "core/constants.h"
#include <variant>
#include <cmath>
#include <limits>

static double eval(const std::string& expr, double x = 0.0, double y = 0.0) {
    Parser parser(expr);
    ParseResult result = parser.parse();
    const ParsedFunction& fn = std::get<ParsedFunction>(result);
    return Evaluator::evaluate(*fn.expr, x, y);
}

static bool ineqCheck(const std::string& expr, double x, double y) {
    Parser parser(expr);
    ParseResult result = parser.parse();
    const ParsedInequality& ineq = std::get<ParsedInequality>(result);
    return Evaluator::satisfies(ineq, x, y);
}

// multiple tests for basic mathematical functions (below)
TEST(EvaluatorTest, BasicArithmetic) {
    EXPECT_DOUBLE_EQ(eval("y = 2 + 3"), 5.0);
    EXPECT_DOUBLE_EQ(eval("y = 10 - 4"), 6.0);
    EXPECT_DOUBLE_EQ(eval("y = 3 * 7"), 21.0);
    EXPECT_DOUBLE_EQ(eval("y = 15 / 3"), 5.0);
}

TEST(EvaluatorTest, Power) {
    EXPECT_DOUBLE_EQ(eval("y = 2^10"), 1024.0);
}

TEST(EvaluatorTest, SinFunction) {
    EXPECT_NEAR(eval("y = sin(pi/2)"), 1.0, 1e-10);
}

TEST(EvaluatorTest, CosFunction) {
    EXPECT_NEAR(eval("y = cos(0)"), 1.0, 1e-10);
}

TEST(EvaluatorTest, TanFunction) {
    EXPECT_NEAR(eval("y = tan(0)"), 0.0, 1e-10);
}