#pragma once

#include <string>
#include <cstddef>

// тут перечисления для токенов
enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Star,
    Slash,
    Caret,
    LParen,
    RParen,
    Comma,
    Greater,
    Less,
    GreaterEqual,
    LessEqual,
    Equal,
    EndOfInput
};

struct Token {
    TokenType type;
    std::string value;
    std::size_t position;

    Token(TokenType t, std::string v, std::size_t pos)
        : type(t), value(std::move(v)), position(pos) {}
};

// comparison operators for inequalities
enum class CompareOp {
    Greater,
    Less,
    GreaterEqual,
    LessEqual
};

// преобразует CompareOp в строку для вывода
inline std::string compareOpToString(CompareOp op) {
    if (op == CompareOp::Greater) return ">";
    if (op == CompareOp::Less) return "<";
    if (op == CompareOp::GreaterEqual) return ">=";
    if (op == CompareOp::LessEqual) return "<=";
    return "?";
}
