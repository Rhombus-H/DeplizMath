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

    while (m_pos < m_input.size() && m_input[m_pos] >= '0' && m_input[m_pos] <= '9') {
        m_pos++;
    }

    if (m_pos < m_input.size() && m_input[m_pos] == '.') {
        m_pos++;
        while (m_pos < m_input.size() && m_input[m_pos] >= '0' && m_input[m_pos] <= '9') {
            m_pos++;
        }
    }

    std::string numStr = m_input.substr(start, m_pos - start);
    return Token(TokenType::Number, numStr, start);
}

Token Tokenizer::readIdentifier() {
    std::size_t start = m_pos;

    while (m_pos < m_input.size()) {
        char ch = m_input[m_pos];
        bool isAlpha = (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
        bool isDigit = (ch >= '0' && ch <= '9');
        bool isUnderscore = (ch == '_');

        if (isAlpha || isDigit || isUnderscore) {
            m_pos++;
        } else {
            break;
        }
    }

    std::string identStr = m_input.substr(start, m_pos - start);
    return Token(TokenType::Identifier, identStr, start);
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

    if (c == '.' && m_pos + 1 < m_input.size()) {
        char nextCh = m_input[m_pos + 1];
        if (nextCh >= '0' && nextCh <= '9') {
            return readNumber();
        }
    }

    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
        return readIdentifier();
    }

    if (c == '=') {
        m_pos++;
        return Token(TokenType::Equal, "=", m_pos - 1);
    }

    if (c == '>') {
        m_pos++;
        // БАГ: проверяем следующий символ ДО инкремента позиции
        if (m_pos < m_input.size() && m_input[m_pos] == '=') {
            m_pos++;
            return Token(TokenType::GreaterEqual, ">=", m_pos - 2);
        }
        return Token(TokenType::Greater, ">", m_pos - 1);
    }

    if (c == '<') {
        m_pos++;
        if (m_pos < m_input.size() && m_input[m_pos] == '=') {
            m_pos++;
            return Token(TokenType::LessEqual, "<=", m_pos - 2);
        }
        return Token(TokenType::Less, "<", m_pos - 1);
    }

    m_pos++;

    if (c == '+') return Token(TokenType::Plus, "+", m_pos - 1);
    if (c == '-') return Token(TokenType::Minus, "-", m_pos - 1);
    if (c == '*') return Token(TokenType::Star, "*", m_pos - 1);
    if (c == '/') return Token(TokenType::Slash, "/", m_pos - 1);
    if (c == '^') return Token(TokenType::Caret, "^", m_pos - 1);
    if (c == '(') return Token(TokenType::LParen, "(", m_pos - 1);
    if (c == ')') return Token(TokenType::RParen, ")", m_pos - 1);
    if (c == ',') return Token(TokenType::Comma, ",", m_pos - 1);

    throw UnexpectedTokenError(std::string(1, c), m_pos - 1);
}
