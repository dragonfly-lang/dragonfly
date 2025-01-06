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
