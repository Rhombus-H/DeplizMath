#pragma once

#include <string>
#include "core/ast.h"
#include "parser/tokenizer.h"

// рекурсивный спуск
class Parser {
public:
    explicit Parser(const std::string& input);

    ParseResult parse();
    ExprPtr parseExpression(); // парсит одно выражение

private:
    Tokenizer tok;
    Token current_token;

    void advance();
    void expect(TokenType type);

    // grammar rules
    ExprPtr parseExpr();   // expr -> term (('+' | '-') term)*
    ExprPtr parseTerm();   // term -> unary (('*' | '/') unary)*
    ExprPtr parseUnary();  // unary -> '-' unary | power
    ExprPtr parsePower();  // power -> atom ('^' unary)   right assoc
    ExprPtr parseAtom();   // atom -> number | ident | func(args) | (expr)
    ExprPtr parseFunctionCall(const std::string& funcName);
};
