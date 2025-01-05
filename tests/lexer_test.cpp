#include <gtest/gtest.h>
#include <memory>

#include "dragon/lexer.h"
#include "dragon/token.h"

// <identifier>
TEST(LexerTests, Identifier) {
    const std::array<std::string, 9> input = {
        "x", 
        "x_y123",
        "reallyLongVariableNameWithNoNumbersOrUnderscores",
        "U_ND_ER_SCO_RES",
        "____starting___with__underscore",
        "2thisShouldError_",        // Identifiers cannot start with a number 
        "this should also error",   // Variables cannot contain spaces
        "Error?",                   // Identifiers cannot contain question marks
        "#*&$£!!!",                 // Identifiers cannot contain any of these symbols
    };
    const std::vector<Token> validTokens = {
        Token(TokenType::Identifier, "x"),                                 
        Token(TokenType::Identifier, "x_y123"),                            
        Token(TokenType::Identifier, "reallyLongVariableNameWithNoNumbersOrUnderscores"), 
        Token(TokenType::Identifier, "U_ND_ER_SCO_RES"),                  
        Token(TokenType::Identifier, "____starting___with__underscore"),  
        Token(TokenType::Unknown, "2thisShouldError_"),                      
        Token(TokenType::Unknown, "this should also error"),               
        Token(TokenType::Unknown, "Error?"),                            
        Token(TokenType::Unknown, "#*&$£!!!")
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_identifier(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// <keyword>
TEST(LexerTests, Keywords) {
    const std::array<std::string, 9> input = {
        "let",
        "mut",
        "if",
        "else",
        "for",
        "in",
        "true",
        "false",
        "while",
    };
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Mut, "mut"),
        Token(TokenType::If, "if"),
        Token(TokenType::Else, "else"),
        Token(TokenType::For, "for"),
        Token(TokenType::In, "in"),
        Token(TokenType::True, "true"),
        Token(TokenType::False, "false"),
        Token(TokenType::While, "while"),
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_identifier(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }    
}

// <string>
TEST(LexerTests, StringLiterals) {
    const std::array<std::string, 3> input = {
        "\"Enter username: \"",
        "\"This is a string with a escape characters \\\" \\n \\t \"",
        "\"Abcdefghijklmnopqrstuvwxyz @#][{};;@'><,.//?)(*&^%$£1234567890+_-=`¬\\|\""

    };
    const std::array<Token, 3> validTokens = {
        Token(TokenType::StringLiteral, "Enter username: "),
        Token(TokenType::StringLiteral, "This is a string with a escape characters \" \n \t "),
        Token(TokenType::StringLiteral, "Abcdefghijklmnopqrstuvwxyz @#][{};;@'><,.//?)(*&^%$£1234567890+_-=`¬\\|")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_string(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// <integer>
TEST(LexerTests, Integer) {
    const std::array<std::string, 4> input = {
        "1",
        "123",
        "0",
        "1_000_000",    // Underscores are allowed, but are ignored
    };
    const std::array<Token, 4> validTokens = {
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::IntegerLiteral, "123"),
        Token(TokenType::IntegerLiteral, "0"),
        Token(TokenType::IntegerLiteral, "1_000_000")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_number(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// Test for all symbols
TEST(LexerTests, Symbols) {
    const std::array<std::string, 18> input = {
        "+",
        "-",
        "*",
        "/",
        "=",
        "==",
        "!=",
        "<",
        ">",
        "<=",
        ">=",
        "&&",
        "||",
        "!",
        "&",
        "|",
        "^",
        "~"
    };
    const std::array<Token, 18> validTokens = {
        Token(TokenType::Plus, "+"),
        Token(TokenType::Minus, "-"),
        Token(TokenType::Star, "*"),
        Token(TokenType::Slash, "/"),
        Token(TokenType::Assign, "="),
        Token(TokenType::Equals, "=="),
        Token(TokenType::NotEquals, "!="),
        Token(TokenType::LessThan, "<"),
        Token(TokenType::GreaterThan, ">"),
        Token(TokenType::LessThanOrEqualTo, "<="),
        Token(TokenType::GreaterThanOrEqualTo, ">="),
        Token(TokenType::And, "&&"),
        Token(TokenType::Or, "||"),
        Token(TokenType::Not, "!"),
        Token(TokenType::Ampersand, "&"),
        Token(TokenType::Pipe, "|"),
        Token(TokenType::Caret, "^"),
        Token(TokenType::Tilde, "~")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_symbol(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// Test for single-line comments
TEST(LexerTests, SingleLineComments) {
    const std::array<std::string, 1> input = {
        "// This is a comment"
    };
    const std::array<Token, 1> validTokens = {
        Token(TokenType::Comment, "// This is a comment")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_comment(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// Test for multi-line comments
TEST(LexerTests, MultiLineComments) {
    const std::array<std::string, 1> input = {
        "/* This is a multi-line comment */"
    };
    const std::array<Token, 1> validTokens = {
        Token(TokenType::Comment, "/* This is a multi-line comment */")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_comment(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// <expr> 
// Arithmetic
TEST(LexerTests, Arithmetic) {
    const std::array<std::string, 4> input = {
        "1 + 2",
        "1 - 2",
        "1 * 2",
        "1 / 2"
    };
    const std::array<std::vector<Token>, 4> validTokens = {
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Plus, "+"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Minus, "-"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Star, "*"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Slash, "/"),
            Token(TokenType::IntegerLiteral, "2")
        }
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        std::vector<Token> tokens = lexer.lex(input[i]);
        ASSERT_TRUE(validTokens[i] == tokens);
    }
}

// <expr> 
// Boolean
TEST(LexerTests, Boolean) {
    const std::array<std::string, 4> input = {
        "true && false",
        "true || false",
        "!true",
        "true == false"
        "true != false"
    };
    const std::array<std::vector<Token>, 4> validTokens = {
        {
            Token(TokenType::True, "true"),
            Token(TokenType::And, "&&"),
            Token(TokenType::False, "false")
        },
        {
            Token(TokenType::True, "true"),
            Token(TokenType::Or, "||"),
            Token(TokenType::False, "false")
        },
        {
            Token(TokenType::Bang, "!"),
            Token(TokenType::True, "true")
        },
        {
            Token(TokenType::True, "true"),
            Token(TokenType::EqualEqual, "=="),
            Token(TokenType::False, "false")
        }
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        std::vector<Token> tokens = lexer.lex(input[i]);
        ASSERT_TRUE(validTokens[i] == tokens);
    }
}

// <expr> 
// Relational
TEST(LexerTests, Relational) {
    const std::array<std::string, 4> input = {
        "1 < 2",
        "1 > 2",
        "1 <= 2",
        "1 >= 2"
    };
    const std::array<std::vector<Token>, 4> validTokens = {
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Less, "<"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Greater, ">"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::LessEqual, "<="),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::GreaterEqual, ">="),
            Token(TokenType::IntegerLiteral, "2")
        }
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        std::vector<Token> tokens = lexer.lex(input[i]);
        ASSERT_TRUE(validTokens[i] == tokens);
    }
}

// <expr> 
// bitwise
TEST(LexerTests, Bitwise) {
    const std::array<std::string, 4> input = {
        "1 & 2",
        "1 | 2",
        "1 ^ 2",
        "~1"
    };
    const std::array<std::vector<Token>, 4> validTokens = {
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Ampersand, "&"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Pipe, "|"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::IntegerLiteral, "1"),
            Token(TokenType::Caret, "^"),
            Token(TokenType::IntegerLiteral, "2")
        },
        {
            Token(TokenType::Tilde, "~"),
            Token(TokenType::IntegerLiteral, "1")
        }
    };
    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        std::vector<Token> tokens = lexer.lex(input[i]);
        ASSERT_TRUE(validTokens[i] == tokens);
    }
}

// <expr>
// Mixed
TEST(LexerTests, Mixed) {
    const std::string input = "1 + 2 * 3 / 4 - 5 == !true && 7 < 8 || 9 > 10 && 11 <= 12 | 13 & 14 ^ 15";  
    const std::vector<Token> validTokens = {
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2"),
        Token(TokenType::Star, "*"),
        Token(TokenType::IntegerLiteral, "3"),
        Token(TokenType::Slash, "/"),
        Token(TokenType::IntegerLiteral, "4"),
        Token(TokenType::Minus, "-"),
        Token(TokenType::IntegerLiteral, "5"),
        Token(TokenType::EqualEqual, "=="),
        Token(TokenType::Bang, "!"),
        Token(TokenType::True, "true"),
        Token(TokenType::And, "&&"),
        Token(TokenType::IntegerLiteral, "7"),
        Token(TokenType::Less, "<"),
        Token(TokenType::IntegerLiteral, "8"),
        Token(TokenType::Or, "||"),
        Token(TokenType::IntegerLiteral, "9"),
        Token(TokenType::Greater, ">"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::And, "&&"),
        Token(TokenType::IntegerLiteral, "11"),
        Token(TokenType::LessEqual, "<="),
        Token(TokenType::IntegerLiteral, "12"),
        Token(TokenType::Pipe, "|"),
        Token(TokenType::IntegerLiteral, "13"),
        Token(TokenType::Ampersand, "&"),
        Token(TokenType::IntegerLiteral, "14"),
        Token(TokenType::Caret, "^"),
        Token(TokenType::IntegerLiteral, "15")
    };

    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let <identifier> <identifier>
TEST(LexerTests, VariableDeclarationWithoutExpr) {
    const std::string input = "let variable int";
    const std::vector<Token>  validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Identifier, "int")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let <identifier> <identifier> = <expr>
TEST (LexerTests, VariableDeclarationWithExpr) {
    const std::string input = "let variable int = 1 + 2";
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Identifier, "int"),
        Token(TokenType::Equal, "="),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let <identifier> = <expr>
TEST(LexerTests, VariableDeclarationWithoutType) {
    const std::string input = "let variable = 1 + 2";
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Equal, "="),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let mut <identifier> <identifier> = <expr>
TEST(LexerTests, MutableVariableDeclarationWithExpr) {
    const std::string input = "let mut variable int = 1 + 2";
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Mut, "mut"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Identifier, "int"),
        Token(TokenType::Equal, "="),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let mut <identifier> <identifier> 
TEST(LexerTests, MutableVariableDeclarationWithoutExpr) {
    const std::string input = "let mut variable int";
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Mut, "mut"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Identifier, "int")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let mut <identifier> = <expr>
TEST(LexerTests, MutableVariableDeclarationWithoutType) {
    const std::string input = "let mut variable = 1 + 2";
    const std::vector<Token> validTokens = {
        Token(TokenType::Let, "let"),
        Token(TokenType::Mut, "mut"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Equal, "="),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// <identifier> = <expr>
TEST(LexerTests, Assignment) {
    const std::string input = "variable = 1 + 2";
    const std::vector<Token> validTokens = {
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Equal, "="),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "2")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// <args>
// Example: x, y, 1 + 3
TEST(LexerTests, Arguments) {
    const std::string input = "x, y, 1 + 3";
    const std::vector<Token> validTokens = {
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Comma, ","),
        Token(TokenType::Identifier, "y"),
        Token(TokenType::Comma, ","),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "3")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// <identifier>(<args>)
TEST(LexerTests, FunctionCall) {
    const std::string input = "function(x, y, 1 + 3)";
    const std::vector<Token> validTokens = {
        Token(TokenType::Identifier, "function"),
        Token(TokenType::LeftParen, "("),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Comma, ","),
        Token(TokenType::Identifier, "y"),
        Token(TokenType::Comma, ","),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "3"),
        Token(TokenType::RightParen, ")")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// while <expr> { <statement(s)> }
TEST(LexerTests, WhileLoop) {
    const std::string input = "while x < 10 { x = x + 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::While, "while"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Less, "<"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// for <identifier> in <identifier> { <statement(s)> }
TEST(LexerTests, ForLoop) {
    const std::string input = "for i in 0..10 { i = i + 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::For, "for"),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::In, "in"),
        Token(TokenType::IntegerLiteral, "0"),
        Token(TokenType::Range, ".."),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// for <identifier> in <expr> { <statement(s)> }
TEST(LexerTests, ForLoopWithExpr) {
    const std::string input = "for i in 0..(10 + 1) { i = i + 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::For, "for"),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::In, "in"),
        Token(TokenType::IntegerLiteral, "0"),
        Token(TokenType::Range, ".."),
        Token(TokenType::LeftParen, "("),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightParen, ")"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "i"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// if <expr> { <statement(s)> }
TEST(LexerTests, IfStatement) {
    const std::string input = "if x < 10 { x = x + 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::If, "if"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Less, "<"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// else if <expr> { <statement(s)> }
TEST(LexerTests, ElseIfStatement) {
    const std::string input = "if x < 10 { x = x + 1 } else if x > 10 { x = x - 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::If, "if"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Less, "<"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}"),
        Token(TokenType::Else, "else"),
        Token(TokenType::If, "if"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Greater, ">"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Minus, "-"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// else { <statement(s)> }
TEST(LexerTests, ElseStatement) {
    const std::string input = "if x < 10 { x = x + 1 } else { x = x - 1 }";
    const std::vector<Token> validTokens = {
        Token(TokenType::If, "if"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Less, "<"),
        Token(TokenType::IntegerLiteral, "10"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Plus, "+"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}"),
        Token(TokenType::Else, "else"),
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Equal, "="),
        Token(TokenType::Identifier, "x"),
        Token(TokenType::Minus, "-"),
        Token(TokenType::IntegerLiteral, "1"),
        Token(TokenType::RightBrace, "}")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}