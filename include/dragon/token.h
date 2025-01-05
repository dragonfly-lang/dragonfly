#pragma once

#include <string>
#include <format>

enum class TokenType;

class Token {
public: 
    TokenType type;
    std::string value;
    size_t line;
    size_t column;

    Token(TokenType type, std::string value, size_t line, size_t column) {
        this->type = type;
        this->value = value;
        this->line = line;
        this->column = column;
    }

    Token(TokenType type, std::string value) {
        this->type = type;
        this->value = value;
        this->line = 0;
        this->column = 0;
    }

    Token(TokenType type) {
        this->type = type;
        this->value = "";
        this->line = 0;
        this->column = 0;
    }

    inline bool operator==(const Token& other) const {
        return this->type == other.type && this->value == other.value;
    }

    inline bool operator!=(const Token& other) const {
        return this->type != other.type || this->value != other.value;
    }

    inline std::string to_string() {
        if (this->line == 0 && this->column == 0 && this->value == "") {
            return std::format("Token({})", this->type);
        }

        if (this->line == 0 && this->column == 0) {
            return std::format("Token({}, {})", this->type, this->value);
        } 

        if (this->value == "") {
            return std::format("Token({}, {}, {}, {})", this->type, this->line, this->column);
        }

        return std::format("Token({}, {}, {}, {})", this->type, this->value, this->line, this->column);
    }
};

enum class TokenType {
    // Keywords
    Let,
    Mut,
    If,
    Else,
    While,
    For,
    In,
    True,
    False,

    // Literals
    IntegerLiteral,
    StringLiteral,
    Identifier,

    // Symbols
    Plus,
    Minus,
    Star,
    Slash,
    And,
    Or,
    Not,
    Equals,
    NotEquals,
    LessThan,
    GreaterThan,
    LessThanOrEqualTo,
    GreaterThanOrEqualTo,
    Assign,
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,
    Comma,
    Dot,
    Range,
    Ampersand,
    Pipe,
    Caret,
    Tilde,

    // Misc 
    Comment,
    Unknown,
};
