#include <gtest/gtest.h>
#include "parser/tokenizer.h"
#include "core/errors.h"

TEST(TokenizerTest, SimpleNumber) {
    Tokenizer t("42");
    Token tok = t.next();
    EXPECT_EQ(tok.type, TokenType::Number);
    EXPECT_EQ(tok.value, "42");
}

TEST(TokenizerTest, DecimalNumber) {
    Tokenizer t("3.14");
    Token tok = t.next();
    EXPECT_EQ(tok.type, TokenType::Number);
    EXPECT_EQ(tok.value, "3.14");
}

TEST(TokenizerTest, Identifier) {
    Tokenizer t("sin");
    Token tok = t.next();
    EXPECT_EQ(tok.type, TokenType::Identifier);
    EXPECT_EQ(tok.value, "sin");
}

TEST(TokenizerTest, Operators) {
    Tokenizer t("+-*/^");
    EXPECT_EQ(t.next().type, TokenType::Plus);
    EXPECT_EQ(t.next().type, TokenType::Minus);
    EXPECT_EQ(t.next().type, TokenType::Star);
    EXPECT_EQ(t.next().type, TokenType::Slash);
    EXPECT_EQ(t.next().type, TokenType::Caret);
}

TEST(TokenizerTest, Parentheses) {
    Tokenizer t("()");
    EXPECT_EQ(t.next().type, TokenType::LParen);
    EXPECT_EQ(t.next().type, TokenType::RParen);
}

TEST(TokenizerTest, ComparisonOperators) {
    Tokenizer t("> < >= <=");
    EXPECT_EQ(t.next().type, TokenType::Greater);
    EXPECT_EQ(t.next().type, TokenType::Less);
    EXPECT_EQ(t.next().type, TokenType::GreaterEqual);
    EXPECT_EQ(t.next().type, TokenType::LessEqual);
}

TEST(TokenizerTest, PeekDoesNotConsume) {
    Tokenizer t("42 + 3");
    std::optional<Token> peeked = t.peek();
    ASSERT_TRUE(peeked.has_value());
    EXPECT_EQ(peeked->type, TokenType::Number);
    EXPECT_EQ(peeked->value, "42");

    Token tok = t.next();
    EXPECT_EQ(tok.type, TokenType::Number);
    EXPECT_EQ(tok.value, "42");
}

TEST(TokenizerTest, EndOfInput) {
    Tokenizer t("");
    Token tok = t.next();
    EXPECT_EQ(tok.type, TokenType::EndOfInput);
}

TEST(TokenizerTest, InvalidCharacterThrows) {
    Tokenizer t("@");
    EXPECT_THROW(t.next(), UnexpectedTokenError);
}

TEST(TokenizerTest, ComplexExpression) {
    Tokenizer t("sin(x) + 2.5 * y");
    std::vector<Token> tokens = t.tokenizeAll();
    ASSERT_GE(tokens.size(), 8u);
    EXPECT_EQ(tokens[0].type, TokenType::Identifier);
    EXPECT_EQ(tokens[0].value, "sin");
    EXPECT_EQ(tokens[1].type, TokenType::LParen);
    EXPECT_EQ(tokens[2].type, TokenType::Identifier);
}

TEST(TokenizerTest, WhitespaceHandling) {
    Tokenizer t("  42  +  x  ");
    Token t1 = t.next();
    EXPECT_EQ(t1.type, TokenType::Number);
    Token t2 = t.next();
    EXPECT_EQ(t2.type, TokenType::Plus);
    Token t3 = t.next();
    EXPECT_EQ(t3.type, TokenType::Identifier);
}
