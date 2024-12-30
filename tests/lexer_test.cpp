#include <gtest/gtest.h>


#include "dragon/lexer.h"
#include "dragon/token.h"


TEST(LexerTests, TokeniseVariableDeclarations) {
    const char* input = R"(
        let variable324534 int = 42
        let mut t_t_3434_t_golden bool = true
        let bacon_wrapped_4_shrimp float = 3.14
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 15);

    int i = 0;

    // let x int = 42
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // x
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);       // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 42

    // let mut y bool = true
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_MUT_KW);       // mut
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BOOL_KW);      // bool
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_TRUE_KW);      // true

    // let z float = 3.14
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // z
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);        // 3.14

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseVariableAssignment) {
    const char* input = "name = \"Alice and the frog\"";
    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 5);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);      // "="
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "42"

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseRelationalOperators) {
    const char* input = "20 > x > 10 < 1000 <= 34_540 >= 0";
    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 6);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "20"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);         // ">"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);         // ">"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "10"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LSS);         // "<"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "1000"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GTE);         // ">="
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "34540"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LTE);         // "<="
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // "0"

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseBooleanOperators) {
    const char* input = "false || true && !!x";
    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 7);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FALSE_KW);    // "false"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_OR);          // "||"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_TRUE_KW);     // "true"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_AND);         // "&&"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_NOT);         // "!"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_NOT);         // "!"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "x"

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseElseIf) {
    const char* input = R"(
        if x > 10 {
            return
        } else if x < 5 {
            break
        }
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 16);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IF_KW);        // "if"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);          // ">"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // "10"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);    // "return"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_ELSE_KW);      // "else"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IF_KW);        // "if"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LTE);          // "<"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // "5"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BREAK_KW);     // "break"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseConditionalForLoop) {
    const char* input = R"(
        if 20 > x > 10 {
            return
        } else {
            for i in 1..10 {
                if i == 5 && !!y {
                    break
                }
                continue
            }
        }
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 36);

    int i = 0;

    // if 20 > x > 10
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IF_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 20
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);          // >
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // x
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);          // >
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 10

    // { return }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // else { for i in 1..10 { ... } }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_ELSE_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FOR_KW);       // for
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // i
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IN_KW);        // in
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 1
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RANGE);        // ..
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 10

    // { if i == 5 && !!y { break } continue }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IF_KW);        // if
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // i
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALITY);     // ==
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);      // 5
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_AND);          // &&
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_NOT);          // !
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_NOT);          // !
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // y

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BREAK_KW);     // break
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_CONTINUE_KW);  // continue
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseWhileLoop) {
    const char* input = R"(
        while y {
            y = false || false
        }
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 10);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_WHILE_KW);     // while
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);       // =
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FALSE_KW);     // false
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_OR);           // ||
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FALSE_KW);     // false
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseFunctionDefinition) {
    const char* input = R"(
        func add(a int, b int) -> int {
            return a + b
        }
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 17);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FUNC_KW);      // func
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // add
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // a
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);       // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // b
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);       // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_CLOSE);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RIGHT_ARROW);  // ->
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);       // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);    // return
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // a
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PLUS);         // +
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);   // b
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseLambdaFunction) {
    const char* input = "let add = (x, y) => x + y";
    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 10);

    int i = 0;

    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);      // "let"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "add"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);      // "="
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_OPEN);  // "("
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);       // ","
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "y"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_CLOSE); // ")"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_DOUBLE_RIGHT_ARROW); // "=>"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "x"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PLUS);        // "+"
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // "y"

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseEmptyInput) {
    const char* input = "";
    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 0);

    free_tokens(tokens);
}

TEST(LexerTests, TokeniseInvalidInput) {
    const char* input = "$/?#¬`";
    TokenList* tokens = tokenise(input);
    

    ASSERT_NE(tokens, nullptr);
    ASSERT_EQ(tokens->count, 1);

    EXPECT_EQ(tokens->tokens[0].type, TOKEN_INVALID); 

    free_tokens(tokens);
}

// This tests everything supported by the lexer, including keywords, operators, and literals.
// The input is a large snippet of Dragon code that uses all the features of the language.
// This is useful to quickly check if the lexer is working as expected, more specific cases are useful for debugging.
TEST(LexerTests, TokeniseEverything) {
    const char* input = R"(
        let x int = 42
        let mut y bool = true
        let z float = 3.14

        if 20 > x > 10 {
            return
        } else {
            for i in 1..10 {
                if i == 5 && !!y {
                    break
                }
                continue
            }
        }

        while y {
            y = false || false
        }

        func add(a int, b int) -> int {
            return a + b
        }

        struct Point {
            x float,
            y float
        }

        let mut p Point = Point{1, 2}
        p.x = p.y * 10 / 100 - 5

        enum Color {
            Red,
            Green,
            Blue,
        }

        Color.Green

        import math
        return;
    )";

    TokenList* tokens = tokenise(input);

    ASSERT_NE(tokens, nullptr);

    ASSERT_EQ(tokens->count, 131); 

    int i = 0;

    // let x int = 42
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);

    // let mut y bool = true
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_MUT_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BOOL_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_TRUE_KW);

    // let z float = 3.14
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);

    // if 20 > x > 10
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IF_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_GRT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);

    // { return }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // else { ... }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_ELSE_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);

    // for i in 1..10
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FOR_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IN_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RANGE);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);

    // Continue for other statements similarly...

    // while y { y = false || false }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_WHILE_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FALSE_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_OR);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FALSE_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // func add(a int, b int) -> int { return a + b }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FUNC_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // add
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // a
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);      // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // b
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);      // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PAREN_CLOSE);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RIGHT_ARROW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INT_KW);      // int
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // a
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_PLUS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // b
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // struct Point { x float, y float }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_STRUCT_KW);   // struct
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Point
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // x
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);       // float
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_FLOAT);       // float
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // let mut p Point = Point{1, 2}
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_LET_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_MUT_KW);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // p
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Point
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Point
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // 1
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // 2
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // p.x = p.y * 10 / 100 - 5
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // p
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_DOT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // x
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_EQUALS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // p
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_DOT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // y
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_ASTERISK);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // 10
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_SLASH);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // 100
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_MINUS);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_INTEGER);     // 5

    // enum Color { Red, Green, Blue }
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_ENUM_KW);     // enum
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Color
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_OPEN);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Red
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Green
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_COMMA);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Blue
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_BRACE_CLOSE);

    // Color.Green
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Color
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_DOT);
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // Green

    // import math
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IMPORT_KW);   // import
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_IDENTIFIER);  // math

    // return;
    EXPECT_EQ(tokens->tokens[i++].type, TOKEN_RETURN_KW);   // return

    free_tokens(tokens);
}



