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
// <identifier>(<expr>)
// while <expr> { <statement(s)> }
// for <identifier> in <identifier> { <statement(s)> }
// for <identifier> in <expr> { <statement(s)> }
// if <expr> { <statement(s)> }
// else { <statement(s)> }
// else if <expr> { <statement(s)> }


