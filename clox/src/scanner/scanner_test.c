#include "scanner.h"

#include "../test_framework/assert.h"
#include "../test_framework/testing.h"

TEST_SUITE(Token)

TEST(Token, tokenLeftParen) {
    initScanner("(");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_LEFT_PAREN, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenRightParen) {
    initScanner(")");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_RIGHT_PAREN, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenLeftBrace) {
    initScanner("{");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_LEFT_BRACE, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenRightBrace) {
    initScanner("}");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_RIGHT_BRACE, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenComma) {
    initScanner(",");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_COMMA, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenDot) {
    initScanner(".");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_DOT, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenMinus) {
    initScanner("-");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_MINUS, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenPlus) {
    initScanner("+");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_PLUS, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenSemicolon) {
    initScanner(";");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_SEMICOLON, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenSlash) {
    initScanner("/");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_SLASH, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenStar) {
    initScanner("*");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_STAR, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenBang) {
    initScanner("!");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_BANG, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenBangEqual) {
    initScanner("!=");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_BANG_EQUAL, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenEqual) {
    initScanner("=");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_EQUAL, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenEqualEqual) {
    initScanner("==");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_EQUAL_EQUAL, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenGreater) {
    initScanner(">");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_GREATER, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenGreaterEqual) {
    initScanner(">=");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_GREATER_EQUAL, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenLess) {
    initScanner("<");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_LESS, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenLessEqual) {
    initScanner("<=");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_LESS_EQUAL, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

//
// Literals
//

TEST(Token, tokenIdentifier) {
    ASSERT_FAIL("not implemented: TOKEN_IDENTIFIER");
}

TEST(Token, tokenString) {
    initScanner("\"hello\"");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_STRING, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenNumber) {
    initScanner("123");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_NUMBER, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

TEST(Token, tokenNumberFloat) {
    initScanner("123.456");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_NUMBER, token.type);
    ASSERT_EQ(TOKEN_EOF, scanToken().type);
}

//
// Keywords
//

TEST(Token, tokenAnd) {
    ASSERT_FAIL("not implemented: TOKEN_AND");
}

TEST(Token, tokenClass) {
    ASSERT_FAIL("not implemented: TOKEN_CLASS");
}

TEST(Token, tokenElse) {
    ASSERT_FAIL("not implemented: TOKEN_ELSE");
}

TEST(Token, tokenFalse) {
    ASSERT_FAIL("not implemented: TOKEN_FALSE");
}

TEST(Token, tokenFor) {
    ASSERT_FAIL("not implemented: TOKEN_FOR");
}

TEST(Token, tokenFun) {
    ASSERT_FAIL("not implemented: TOKEN_FUN");
}

TEST(Token, tokenIf) {
    ASSERT_FAIL("not implemented: TOKEN_IF");
}

TEST(Token, tokenNil) {
    ASSERT_FAIL("not implemented: TOKEN_NIL");
}

TEST(Token, tokenOr) {
    ASSERT_FAIL("not implemented: TOKEN_OR");
}

TEST(Token, tokenPrint) {
    ASSERT_FAIL("not implemented: TOKEN_PRINT");
}

TEST(Token, tokenReturn) {
    ASSERT_FAIL("not implemented: TOKEN_RETURN");
}

TEST(Token, tokenSuper) {
    ASSERT_FAIL("not implemented: TOKEN_SUPER");
}

TEST(Token, tokenThis) {
    ASSERT_FAIL("not implemented: TOKEN_THIS");
}

TEST(Token, tokenTrue) {
    ASSERT_FAIL("not implemented: TOKEN_TRUE");
}

TEST(Token, tokenVar) {
    ASSERT_FAIL("not implemented: TOKEN_VAR");
}

TEST(Token, tokenWhile) {
    ASSERT_FAIL("not implemented: TOKEN_WHILE");
}

//
// Special tokens
//

TEST(Token, tokenError) {
    initScanner("@");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_ERROR, token.type);
}

TEST(Token, tokenEof) {
    initScanner("");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_EOF, token.type);
}

//
// Whitespace and comments
//

TEST(Token, skipWhitespace) {
    initScanner("   +");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_PLUS, token.type);
}

TEST(Token, skipNewline) {
    initScanner("\n\n+");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_PLUS, token.type);
}

TEST(Token, skipComment) {
    initScanner("// comment\n+");
    Token token = scanToken();
    ASSERT_EQ(TOKEN_PLUS, token.type);
}
