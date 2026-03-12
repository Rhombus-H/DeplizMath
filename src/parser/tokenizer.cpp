#include "parser/tokenizer.h"
#include <cctype>

Tokenizer::Tokenizer(const std::string& input)
    : m_input(input), m_pos(0) {}

void Tokenizer::skipWhitespace() {
    while (m_pos < m_input.size()) {
        char c = m_input[m_pos];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            m_pos++;
        } else {
            break;
        }
    }
}

Token Tokenizer::next() {
    skipWhitespace();

    if (m_pos >= m_input.size()) {
        return Token(TokenType::EndOfInput, "", m_pos);
    }

    char c = m_input[m_pos];
    m_pos++;

    if (c == '+') return Token(TokenType::Plus, "+", m_pos - 1);
    if (c == '-') return Token(TokenType::Minus, "-", m_pos - 1);

    return Token(TokenType::Unknown, std::string(1, c), m_pos - 1);
}