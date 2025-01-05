#pragma once

#include <string>

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

inline std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::Let: return "Let";
        case TokenType::Mut: return "Mut";
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::While: return "While";
        case TokenType::For: return "For";
        case TokenType::In: return "In";
        case TokenType::True: return "True";
        case TokenType::False: return "False";
        case TokenType::IntegerLiteral: return "IntegerLiteral";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::Identifier: return "Identifier";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
        case TokenType::And: return "And";
        case TokenType::Or: return "Or";
        case TokenType::Not: return "Not";
        case TokenType::Equals: return "Equals";
        case TokenType::NotEquals: return "NotEquals";
        case TokenType::LessThan: return "LessThan";
        case TokenType::GreaterThan: return "GreaterThan";
        case TokenType::LessThanOrEqualTo: return "LessThanOrEqualTo";
        case TokenType::GreaterThanOrEqualTo: return "GreaterThanOrEqualTo";
        case TokenType::Assign: return "Assign";
        case TokenType::LeftParen: return "LeftParen";
        case TokenType::RightParen: return "RightParen";
        case TokenType::LeftBrace: return "LeftBrace";
        case TokenType::RightBrace: return "RightBrace";
        case TokenType::LeftBracket: return "LeftBracket";
        case TokenType::RightBracket: return "RightBracket";
        case TokenType::Comma: return "Comma";
        case TokenType::Dot: return "Dot";
        case TokenType::Range: return "Range";
        case TokenType::Ampersand: return "Ampersand";
        case TokenType::Pipe: return "Pipe";
        case TokenType::Caret: return "Caret";
        case TokenType::Tilde: return "Tilde";
        case TokenType::Comment: return "Comment";
        case TokenType::Unknown: return "Unknown";
        default: return "Unknown";
    }
}

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
                std::stringstream ss;
        ss << "Token(" << token_type_to_string(this->type);
        if (!this->value.empty()) {
            ss << ", " << this->value;
        }
        if (this->line != 0 || this->column != 0) {
            ss << ", " << this->line << ", " << this->column;
        }
        ss << ")";
        return ss.str();
    }
};

inline std::string token_vector_to_string(std::vector<Token> tokens) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < tokens.size(); i++) {
        ss << tokens[i].to_string();
        if (i < tokens.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}