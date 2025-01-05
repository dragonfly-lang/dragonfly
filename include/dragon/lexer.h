#pragma once 

#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include "token.h"

class Lexer {
public:
    Lexer() = default;
    Lexer(std::string input);

    std::vector<Token> lex(std::string input);  
    std::vector<Token> lex();

    void reset();

    Token lex_identifier();
    Token lex_number();
    Token lex_string();
    Token lex_symbol();
    Token lex_single_line_comment();
    Token lex_multi_line_comment();

    TokenType get_keyword(std::string input);

private:
    std::vector<Token> tokens;
    std::string input;
    size_t index = 0;
    size_t line = 1;
    size_t column = 1;

    std::optional<char> peek() const;
    std::optional<char> peek_next() const;
    std::optional<char> advance();
};