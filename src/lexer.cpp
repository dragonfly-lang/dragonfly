#include "dragon/lexer.h"

#include <iostream>

Lexer::Lexer(std::string input) {
    this->input = input;
}

std::vector<Token> Lexer::lex(std::string input) {
    if (!this->input.empty()) this->reset();
    this->input = input;
    return this->lex();
}

void Lexer::reset() {
    this->tokens.clear();
    this->input = "";
    this->index = 0;
    this->line = 1;
    this->column = 1;
    }

std::vector<Token> Lexer::lex() {
    while (this->index < this->input.size()) {
        auto opt_c = this->peek();
        if (!opt_c.has_value()) break;
        char c = opt_c.value();
        std::cout << "lexing starting with: " << c << std::endl;

        if (std::isspace(c)) {
            this->advance();
            continue;
        }

        if (std::isalpha(c) || c == '_') {
            this->tokens.push_back(this->lex_identifier());
            continue;
        }

        if (std::isdigit(c)) {
            this->tokens.push_back(this->lex_number());
            continue;
        }

        if (c == '"') {
            std::cout << "lexing string" << std::endl;
            this->tokens.push_back(this->lex_string());
            continue;
        }

        if (c == '/' && this->peek_next() == '/') {
            this->tokens.push_back(this->lex_single_line_comment());
            continue;
        }

        if (c == '/' && this->peek_next() == '*') {
            this->tokens.push_back(this->lex_multi_line_comment());
            continue;
    }

        this->tokens.push_back(this->lex_symbol());
    }

    return this->tokens;
}

Token Lexer::lex_identifier() {
    std::string value = "";
    size_t line = this->line;
    size_t column = this->column;

    while (true) {
        auto opt_c = this->peek();
        if (!opt_c.has_value() || !(std::isalnum(opt_c.value()) || opt_c.value() == '_')) {
            break;
        }
        value += this->advance().value();
        std::cout << "building value: " << value << std::endl;
    }

    TokenType type = this->get_keyword(value);
    std::cout << "type: " << token_type_to_string(type) << std::endl;
    std::cout << "value: " << "\"" << value << "\"" << std::endl;
    return Token(type, value, line, column);
    }

Token Lexer::lex_number() {
    std::string value = "";
    size_t line = this->line;
    size_t column = this->column;

    while (true) {
        auto opt_c = this->peek();
        if (opt_c.has_value() && opt_c.value() == '_') {
            this->advance();
            continue;
        }
        if (!opt_c.has_value() || !std::isdigit(opt_c.value())) {
            break;
        }
        value += this->advance().value();
    }

    return Token(TokenType::IntegerLiteral, value, line, column);
}

Token Lexer::lex_string() {
    std::string value = "";
    size_t line = this->line;
    size_t column = this->column;

    this->advance(); // Skip the opening quote

    while (true) {
        auto opt_c = this->peek();
        if (!opt_c.has_value() || opt_c.value() == '"') {
            break;
        }

        if (opt_c.value() == '\\') {
            this->advance(); // Skip the backslash
            auto escaped_char = this->advance();
            if (escaped_char.has_value()) {
                switch (escaped_char.value()) {
                    case 'n': value += '\n'; break;
                    case 't': value += '\t'; break;
                    case 'r': value += '\r'; break;
                    case '\\': value += '\\'; break;
                    case '"': value += '"'; break;
                    default: value += '\\'; value += escaped_char.value(); break;
                }
            }
        } else {
            value += this->advance().value();
        }
    }

    this->advance(); // Skip the closing quote

    return Token(TokenType::StringLiteral, value, line, column);
}

Token Lexer::lex_symbol() {
    std::string value = "";
    size_t line = this->line;
    size_t column = this->column;

    auto opt_c = this->peek();
    if (!opt_c.has_value()) {
        return Token(TokenType::Unknown, value, line, column);
    }

    char c = opt_c.value();
    switch (c) {
        case '+':
            value += this->advance().value();
            return Token(TokenType::Plus, value, line, column);
        case '-':
            value += this->advance().value();
            return Token(TokenType::Minus, value, line, column);
        case '*':
            value += this->advance().value();
            return Token(TokenType::Star, value, line, column);
        case '/':
            value += this->advance().value();
            return Token(TokenType::Slash, value, line, column);
        case '!':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '=') {
                value += this->advance().value();
                return Token(TokenType::NotEquals, value, line, column);
            }
            return Token(TokenType::Not, value, line, column);
        case '=':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '=') {
                value += this->advance().value();
                return Token(TokenType::Equals, value, line, column);
            }
            return Token(TokenType::Assign, value, line, column);
        case '<':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '=') {
                value += this->advance().value();
                return Token(TokenType::LessThanOrEqualTo, value, line, column);
            }
            return Token(TokenType::LessThan, value, line, column);
        case '>':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '=') {
                value += this->advance().value();
                return Token(TokenType::GreaterThanOrEqualTo, value, line, column);
            }
            return Token(TokenType::GreaterThan, value, line, column);
        case '&':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '&') {
                value += this->advance().value();
                return Token(TokenType::And, value, line, column);
            }
            return Token(TokenType::Ampersand, value, line, column);
        case '|':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '|') {
                value += this->advance().value();
                return Token(TokenType::Or, value, line, column);
            }
            return Token(TokenType::Pipe, value, line, column);
        case '^':
            value += this->advance().value();
            return Token(TokenType::Caret, value, line, column);
        case '~':
            value += this->advance().value();
            return Token(TokenType::Tilde, value, line, column);
        case '(':
            value += this->advance().value();
            return Token(TokenType::LeftParen, value, line, column);
        case ')':
            value += this->advance().value();
            return Token(TokenType::RightParen, value, line, column);
        case '{':
            value += this->advance().value();
            return Token(TokenType::LeftBrace, value, line, column);
        case '}':
            value += this->advance().value();
            return Token(TokenType::RightBrace, value, line, column);
        case '[':
            value += this->advance().value();
            return Token(TokenType::LeftBracket, value, line, column);
        case ']':
            value += this->advance().value();
            return Token(TokenType::RightBracket, value, line, column);
        case ',':
            value += this->advance().value();
            return Token(TokenType::Comma, value, line, column);
        case '.':
            value += this->advance().value();
            if (auto next = this->peek(); next.has_value() && next.value() == '.') {
                value += this->advance().value();
                return Token(TokenType::Range, value, line, column);
            }
            return Token(TokenType::Dot, value, line, column);
        default:
            value += this->advance().value();
            return Token(TokenType::Unknown, value, line, column);
    }
}
            break;
        case '.':
            printf("Next char: %c\n", lexer->source[lexer->position + 1]);
            if (lexer->source[lexer->position + 1] == '.') {
                type = TOKEN_RANGE;
                value = strndup(lexer->source + lexer->position, 2);
                lexer->position++;
            } else {
                type = TOKEN_INVALID;
                value = strndup(lexer->source + lexer->position, 1);
            }
            break;
        default:
            type = TOKEN_INVALID;
            value = strndup(lexer->source + lexer->position, 1);
            break;
    }

    lexer->position++;

    Token token = {type, value};
    return token;
}

TokenType Lexer::get_keyword(std::string value) {
    if (value == "let") {
        return TokenType::Let;
    }

    if (value == "mut") {
        return TokenType::Mut;
    }

    if (value == "if") {
        return TokenType::If;
    }

    if (value == "else") {
        return TokenType::Else;
    }

    if (value == "while") {
        return TokenType::While;
    }

    if (value == "for") {
        return TokenType::For;
    }

    if (value == "in") {
        return TokenType::In;
    }

    if (value == "true") {
        return TokenType::True;
    }

    if (value == "false") {
        return TokenType::False;
    }

    return TokenType::Identifier;
    }

std::optional<char> Lexer::peek() const {
    if (this->index < this->input.size()) {
        return this->input[this->index];
    }
    return std::nullopt;
}

std::optional<char> Lexer::peek_next() const {
    if (this->index + 1 < this->input.size()) {
        return this->input[this->index + 1];
    }
    return std::nullopt;
}

std::optional<char> Lexer::advance() {
    if (this->index < this->input.size()) {
        char c = this->input[this->index];
        this->index++;
        this->column++;

        if (c == '\n') {
            this->line++;
            this->column = 1;
        }

        return c;
    }
    return std::nullopt;
}