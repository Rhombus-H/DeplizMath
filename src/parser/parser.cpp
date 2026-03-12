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

