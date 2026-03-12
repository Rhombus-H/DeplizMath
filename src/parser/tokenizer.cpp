#include "parser/tokenizer.h"
#include "core/errors.h"
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

Token Tokenizer::readNumber() {
    std::size_t start = m_pos;

    // БАГ: не обрабатываем дробную часть правильно
    while (m_pos < m_input.size() && m_input[m_pos] >= '0' && m_input[m_pos] <= '9') {
        m_pos++;
    }

    // пробуем обработать точку (но с багом)
    if (m_pos < m_input.size() && m_input[m_pos] == '.') {
        m_pos++;
    }

    std::string numStr = m_input.substr(start, m_pos - start);
    return Token(TokenType::Number, numStr, start);
}

Token Tokenizer::next() {
    skipWhitespace();

    if (m_pos >= m_input.size()) {
        return Token(TokenType::EndOfInput, "", m_pos);
    }

    char c = m_input[m_pos];

    if (c >= '0' && c <= '9') {
        return readNumber();
    }

    m_pos++;

    if (c == '+') return Token(TokenType::Plus, "+", m_pos - 1);
    if (c == '-') return Token(TokenType::Minus, "-", m_pos - 1);
    if (c == '*') return Token(TokenType::Star, "*", m_pos - 1);
    if (c == '/') return Token(TokenType::Slash, "/", m_pos - 1);

    return Token(TokenType::Unknown, std::string(1, c), m_pos - 1);
}