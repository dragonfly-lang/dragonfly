#include <gtest/gtest.h>

extern "C" {
    #include "pebble/lexer.h"
}

TEST(LexerTests, TokenizeSimpleString) {
    const char* input = "let x = 42;";
    TokenList* tokens = tokenize(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 4);

    EXPECT_EQ(tokens->tokens[0].type, TOKEN_KEYWORD);
    EXPECT_EQ(tokens->tokens[1].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[2].type, TOKEN_OPERATOR);
    EXPECT_EQ(tokens->tokens[3].type, TOKEN_INTEGER);

    free_tokens(tokens);
}
