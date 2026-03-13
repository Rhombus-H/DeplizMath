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

TEST(EvaluatorTest, LogBase10) {
    EXPECT_NEAR(eval("y = log(100)"), 2.0, 1e-10);
}

TEST(EvaluatorTest, NaturalLog) {
    EXPECT_NEAR(eval("y = ln(e)"), 1.0, 1e-10);
}

TEST(EvaluatorTest, Sqrt) {
    EXPECT_DOUBLE_EQ(eval("y = sqrt(16)"), 4.0);
}

TEST(EvaluatorTest, Abs) {
    EXPECT_DOUBLE_EQ(eval("y = abs(-5)"), 5.0);
}

TEST(EvaluatorTest, Exp) {
    EXPECT_NEAR(eval("y = exp(1)"), E, 1e-10);
}

TEST(EvaluatorTest, CeilFloor) {
    EXPECT_DOUBLE_EQ(eval("y = ceil(3.2)"), 4.0);
    EXPECT_DOUBLE_EQ(eval("y = floor(3.8)"), 3.0);
}

TEST(EvaluatorTest, Sign) {
    EXPECT_DOUBLE_EQ(eval("y = sign(5)"), 1.0);
    EXPECT_DOUBLE_EQ(eval("y = sign(-3)"), -1.0);
    EXPECT_DOUBLE_EQ(eval("y = sign(0)"), 0.0);
}

TEST(EvaluatorTest, SinSquaredPlusCosSquared) {
    double values[] = {0.0, 0.5, 1.0, PI / 4.0, PI / 2.0, PI};
    for (int i = 0; i < 6; i++) {
        double x = values[i];
        double result = eval("y = sin(x)^2 + cos(x)^2", x);
        EXPECT_NEAR(result, 1.0, 1e-10);
    }
}

TEST(EvaluatorTest, DivisionByZeroReturnsNaN) {
    double result = eval("y = 1/x", 0.0);
    EXPECT_TRUE(std::isnan(result));
}

TEST(EvaluatorTest, SqrtNegativeReturnsNaN) {
    double result = eval("y = sqrt(-1)");
    EXPECT_TRUE(std::isnan(result));
}

TEST(EvaluatorTest, LogNegativeReturnsNaN) {
    EXPECT_TRUE(std::isnan(eval("y = log(-5)")));
}

TEST(EvaluatorTest, LnZeroReturnsNaN) {
    EXPECT_TRUE(std::isnan(eval("y = ln(0)")));
}

TEST(EvaluatorTest, AsinOutOfRange) {
    EXPECT_TRUE(std::isnan(eval("y = asin(2)")));
}

TEST(EvaluatorTest, SatisfiesGreater) {
    EXPECT_TRUE(ineqCheck("y > x^2", 1.0, 2.0));
    EXPECT_FALSE(ineqCheck("y > x^2", 1.0, 0.0));
}

TEST(EvaluatorTest, SatisfiesLess) {
    EXPECT_TRUE(ineqCheck("y < x + 1", 0.0, 0.0));
    EXPECT_FALSE(ineqCheck("y < x + 1", 0.0, 2.0));
}

TEST(EvaluatorTest, SatisfiesTwoVariable) {
    EXPECT_TRUE(ineqCheck("x^2 + y^2 < 9", 1.0, 1.0));
    EXPECT_FALSE(ineqCheck("x^2 + y^2 < 9", 3.0, 0.0));
}

TEST(EvaluatorTest, DiscontinuityDetection) {
    EXPECT_TRUE(Evaluator::isDiscontinuity(1.0, 1000.0, 20.0));
    EXPECT_FALSE(Evaluator::isDiscontinuity(1.0, 2.0, 20.0));
    double nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_TRUE(Evaluator::isDiscontinuity(nan, 1.0, 20.0));
}

TEST(EvaluatorTest, NiceStep) {
    double step1 = Evaluator::niceStep(20.0);
    EXPECT_TRUE(step1 == 1.0 || step1 == 2.0 || step1 == 5.0);

    double step2 = Evaluator::niceStep(100.0);
    EXPECT_TRUE(step2 == 10.0 || step2 == 20.0);
}

TEST(EvaluatorTest, ContainsVar) {
    Parser p1("y = x^2 + 1");
    ParseResult r1 = p1.parse();
    const ParsedFunction& f1 = std::get<ParsedFunction>(r1);
    EXPECT_TRUE(Evaluator::containsVar(*f1.expr, "x"));
    EXPECT_FALSE(Evaluator::containsVar(*f1.expr, "y"));

    Parser p2("x^2 + y^2 = 1");
    ParseResult r2 = p2.parse();
    const ParsedEquation& eq = std::get<ParsedEquation>(r2);
    EXPECT_TRUE(Evaluator::containsVar(*eq.lhs, "y"));
    EXPECT_TRUE(Evaluator::containsVar(*eq.lhs, "x"));
}