#include "dragon/parser.h"

std::optional<Token> Parser::peek() const {
    if (index < tokens.size()) {
        return tokens[index];
    }

    return std::nullopt;
}

std::optional<Token> Parser::peek_next() const {
    if (index + 1 < tokens.size()) {
        return tokens[index + 1];
    }

    return std::nullopt;
}

std::optional<Token> Parser::advance(TokenType type) {
    if (index < tokens.size() && tokens[index].type == type) {
        return tokens[index++];
    }

    return std::nullopt;
}

void Parser::reset() {
    index = 0;
    tokens.clear();
    nodes.clear();
}

