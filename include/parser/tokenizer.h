#pragma once

#include <optional>
#include <string>
#include <vector>
#include "core/types.h"

class Tokenizer {
public:
    explicit Tokenizer(const std::string& input);

    Token next();
    std::optional<Token> peek(); // смотрим следующий токен не двигая позицию
    bool hasMore() const;
    std::vector<Token> tokenizeAll();

private:
    std::string m_input;
    std::size_t m_pos;

    void skipWhitespace();
    Token readNumber();
    Token readIdentifier();
};
