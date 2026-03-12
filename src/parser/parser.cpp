#include "parser/parser.h"
#include "core/constants.h"
#include "core/errors.h"

Parser::Parser(const std::string& input)
    : tok(input), current_token(tok.next()) {}

void Parser::advance() {
    current_token = tok.next();
}

void Parser::expect(TokenType type) {
    if (current_token.type != type) {
        if (type == TokenType::RParen) {
            throw UnmatchedParenError(current_token.position);
        }
        throw UnexpectedTokenError(current_token.value, current_token.position);
    }
    advance();
}

ParseResult Parser::parse() {
    // парсим левую часть
    ExprPtr leftSide = parseExpr();

    // обязательно должен быть оператор: = > < >= <=
    if (current_token.type == TokenType::EndOfInput) {
        throw ParseError("Expected '=' or comparison operator. Use format: y = f(x) or lhs = rhs");
    }

    // === ЗНАК РАВНО -> функция или уравнение ===
    if (current_token.type == TokenType::Equal) {
        advance();
        ExprPtr rightSide = parseExpr();

        if (current_token.type != TokenType::EndOfInput) {
            throw UnexpectedTokenError(current_token.value, current_token.position);
        }

        // если lhs это просто "y" -> пробуем как функцию
        // (pipeline потом доп. проверит содержит ли rhs y)
        VariableLit* varCheck = std::get_if<VariableLit>(&leftSide->data);
        if (varCheck != nullptr && varCheck->name == "y") {
            // пока считаем что это функция. если rhs содержит y,
            // pipeline переклассифицирует в уравнение
            ParsedFunction func;
            func.expr = std::move(rightSide);
            return func;
        }

        // иначе уравнение (x^2+y^2=1, x=y^2, и тд)
        ParsedEquation eq;
        eq.lhs = std::move(leftSide);
        eq.rhs = std::move(rightSide);
        return eq;
    }

// === НЕРАВЕНСТВО ===
bool isInequality = false;
CompareOp compOp = CompareOp::Less;

if (current_token.type == TokenType::Greater) {
    isInequality = true;
    compOp = CompareOp::Greater;
} else if (current_token.type == TokenType::Less) {
    isInequality = true;
    compOp = CompareOp::Less;
} else if (current_token.type == TokenType::GreaterEqual) {
    isInequality = true;
    compOp = CompareOp::GreaterEqual;
} else if (current_token.type == TokenType::LessEqual) {
    isInequality = true;
    compOp = CompareOp::LessEqual;
}

if (isInequality) {
    advance();
    ExprPtr rightSide = parseExpr();

    if (current_token.type != TokenType::EndOfInput) {
        throw UnexpectedTokenError(current_token.value, current_token.position);
    }

    // определяем тип
    bool isTwoVar = true;
    VariableLit* yCheck = std::get_if<VariableLit>(&leftSide->data);
    if (yCheck != nullptr && yCheck->name == "y") {
        isTwoVar = false;
    }

    ParsedInequality ineq;
    ineq.op = compOp;
    ineq.lhs = std::move(leftSide);
    ineq.rhs = std::move(rightSide);
    ineq.twoVariable = isTwoVar;
    return ineq;
}

throw UnexpectedTokenError(current_token.value, current_token.position);
}