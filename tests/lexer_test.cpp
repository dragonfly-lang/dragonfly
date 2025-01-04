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
    const std::array<Token, 1> validTokens = {
        Token(TokenType::StringLiteral, "Enter username: "),
        Token(TokenType::StringLiteral, "This is a string with a escape characters \" \n \t "),
        Token(TokenType::StringLiteral, "Abcdefghijklmnopqrstuvwxyz @#][{};;@'><,.//?)(*&^%$£1234567890+_-=`¬\\|")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_string_literal(input[i]);
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

// Test for operators
TEST(LexerTests, Operators) {
    const std::array<std::string, 5> input = {
        "+", "-", "*", "/", "="
    };
    const std::array<Token, 5> validTokens = {
        Token(TokenType::Plus, "+"),
        Token(TokenType::Minus, "-"),
        Token(TokenType::Asterisk, "*"),
        Token(TokenType::Slash, "/"),
        Token(TokenType::Equal, "=")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_operator(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// Test for punctuation
TEST(LexerTests, Punctuation) {
    const std::array<std::string, 3> input = {
        "{", "}", ";"
    };
    const std::array<Token, 3> validTokens = {
        Token(TokenType::LeftBrace, "{"),
        Token(TokenType::RightBrace, "}"),
        Token(TokenType::Semicolon, ";")
    };

    Lexer lexer;

    for (size_t i = 0; i < input.size(); i++) {
        Token t = lexer.lex_punctuation(input[i]);
        ASSERT_TRUE(validTokens[i] == t);
    }
}

// <expr> 
// Arithmetic

// <expr> 
// Boolean

// <expr> 
// Relational

// <expr> 
// bitwise

// <expr>
// Mixed

// let <identifier> <identifier> 
TEST(LexerTests, VariableDeclarationWithoutExpr) {
    const std::string input = "let variable int";
    const std::vector<Token>  validTokens = {
        Token(TokenType::LetKeyword, "let"),
        Token(TokenType::Identifier, "variable"),
        Token(TokenType::Identifier, "int")
    };
    Lexer lexer;

    std::vector<Token> tokens = lexer.lex(input);

    ASSERT_TRUE(validTokens == tokens);
}

// let <identifier> <identifier> = <expr>
// let <identifier> = <expr>
// const <identifier> <identifier> = <expr>
// const <identifier> <identifier> 
// const <identifier> = <expr>
// <identifier> = <expr>
// <params>
// <identifier>(<params>)
// while <expr> { <statement(s)> }
// for <identifier> in <identifier> { <statement(s)> }
// for <identifier> in <expr> { <statement(s)> }
// if <expr> { <statement(s)> }
// else if <expr> { <statement(s)> }
// else { <statement(s)> }
