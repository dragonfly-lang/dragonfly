#pragma once

#include "token.h"
#include "ast.h"



class Parser {
private:
    std::vector<Token> tokens;
    std::vector<Node> nodes;
    size_t index;

    std::optional<Token> peek() const;
    std::optional<Token> peek_next() const;
    std::optional<Token> advance(TokenType type);

public: 
    explicit Parser(std::vector<Token> tokens) : tokens(std::move(tokens)), index(0) {}
    explicit Parser() : tokens(std::vector<Token>()), index(0) {}
};