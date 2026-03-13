#include <gtest/gtest.h>
#include "parser/parser.h"
#include "evaluator/evaluator.h"
#include "core/constants.h"
#include <variant>

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
