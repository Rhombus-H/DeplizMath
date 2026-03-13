#include <gtest/gtest.h>
#include "parser/parser.h"
#include "evaluator/evaluator.h"
#include "core/errors.h"
#include <cmath>
#include <variant>

static double parseEval(const std::string& expr, double x = 0.0) {
    Parser parser(expr);
    ParseResult result = parser.parse();
    const ParsedFunction& fn = std::get<ParsedFunction>(result);
    return Evaluator::evaluate(*fn.expr, x);
}

TEST(ParserTest, SimpleNumber) {
    EXPECT_DOUBLE_EQ(parseEval("y = 42"), 42.0);
}

TEST(ParserTest, Addition) {
    EXPECT_DOUBLE_EQ(parseEval("y = 2 + 3"), 5.0);
}

TEST(ParserTest, Subtraction) {
    EXPECT_DOUBLE_EQ(parseEval("y = 10 - 4"), 6.0);
}

TEST(ParserTest, Multiplication) {
    EXPECT_DOUBLE_EQ(parseEval("y = 3 * 7"), 21.0);
}

TEST(ParserTest, Division) {
    EXPECT_DOUBLE_EQ(parseEval("y = 15 / 3"), 5.0);
}

TEST(ParserTest, PrecedenceMulOverAdd) {
    EXPECT_DOUBLE_EQ(parseEval("y = 2 + 3 * 4"), 14.0);
}

TEST(ParserTest, PrecedenceParens) {
    EXPECT_DOUBLE_EQ(parseEval("y = (2 + 3) * 4"), 20.0);
}

TEST(ParserTest, PowerRightAssociative) {
    EXPECT_DOUBLE_EQ(parseEval("y = 2^3^2"), 512.0);
}

TEST(ParserTest, UnaryMinusBelowPower) {
    EXPECT_DOUBLE_EQ(parseEval("y = -x^2", 3.0), -9.0);
}

TEST(ParserTest, UnaryMinusAlone) {
    EXPECT_DOUBLE_EQ(parseEval("y = -5"), -5.0);
}

TEST(ParserTest, DoubleNegation) {
    EXPECT_DOUBLE_EQ(parseEval("y = --5"), 5.0);
}

TEST(ParserTest, FunctionSin) {
    EXPECT_NEAR(parseEval("y = sin(0)"), 0.0, 1e-10);
}

TEST(ParserTest, FunctionCos) {
    EXPECT_NEAR(parseEval("y = cos(0)"), 1.0, 1e-10);
}

TEST(ParserTest, NestedFunctions) {
    EXPECT_NEAR(parseEval("y = sin(cos(0))"), std::sin(1.0), 1e-10);
}

TEST(ParserTest, ConstantPi) {
    EXPECT_NEAR(parseEval("y = pi"), 3.14159265358979, 1e-10);
}

TEST(ParserTest, ConstantE) {
    EXPECT_NEAR(parseEval("y = e"), 2.71828182845904, 1e-10);
}

TEST(ParserTest, VariableX) {
    EXPECT_DOUBLE_EQ(parseEval("y = x", 5.0), 5.0);
}

TEST(ParserTest, ComplexExpression) {
    EXPECT_DOUBLE_EQ(parseEval("y = x^2 + 2*x + 1", 3.0), 16.0);
}

TEST(ParserTest, InequalityParsesCorrectly) {
    Parser parser("y > x^2");
    ParseResult result = parser.parse();
    ASSERT_TRUE(std::holds_alternative<ParsedInequality>(result));
    const ParsedInequality& ineq = std::get<ParsedInequality>(result);
    EXPECT_EQ(ineq.op, CompareOp::Greater);
    EXPECT_FALSE(ineq.twoVariable);
}

TEST(ParserTest, TwoVariableInequality) {
    Parser parser("x^2 + y^2 < 9");
    ParseResult result = parser.parse();
    ASSERT_TRUE(std::holds_alternative<ParsedInequality>(result));
    const ParsedInequality& ineq = std::get<ParsedInequality>(result);
    EXPECT_EQ(ineq.op, CompareOp::Less);
    EXPECT_TRUE(ineq.twoVariable);
}

TEST(ParserTest, EquationParsesCorrectly) {
    Parser parser("x^2 + y^2 = 1");
    ParseResult result = parser.parse();
    ASSERT_TRUE(std::holds_alternative<ParsedEquation>(result));
}

TEST(ParserTest, EquationXequalsYsquared) {
    Parser parser("x = y^2");
    ParseResult result = parser.parse();
    ASSERT_TRUE(std::holds_alternative<ParsedEquation>(result));
}

TEST(ParserTest, UnmatchedParenThrows) {
    EXPECT_THROW(parseEval("y = (2 + 3"), UnmatchedParenError);
}

TEST(ParserTest, UnknownFunctionThrows) {
    EXPECT_THROW(parseEval("y = foo(x)"), UnknownFunctionError);
}

TEST(ParserTest, BareExpressionThrows) {
    EXPECT_THROW({
        Parser p("x^2");
        p.parse();
    }, ParseError);
}
