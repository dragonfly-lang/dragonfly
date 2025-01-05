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

Token lex_symbol(Lexer* lexer) {
    char c = lexer->source[lexer->position];
    TokenType type = TOKEN_INVALID;
    char* value = NULL;

    switch (c) {
        case '=':
            type = TOKEN_EQUALS;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '+':
            type = TOKEN_PLUS;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '{':
            type = TOKEN_BRACE_OPEN;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '}':
            type = TOKEN_BRACE_CLOSE;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '(':
            type = TOKEN_PAREN_OPEN;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case ')':
            type = TOKEN_PAREN_CLOSE;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case ',':
            type = TOKEN_COMMA;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '>':
            type = TOKEN_GRT;
            value = strndup(lexer->source + lexer->position, 1);
            break;
        case '-':
            if (lexer->source[lexer->position + 1] == '>') {
                type = TOKEN_RIGHT_ARROW;
                value = strndup(lexer->source + lexer->position, 2);
                lexer->position++;
            } else {
                type = TOKEN_INVALID;
                value = strndup(lexer->source + lexer->position, 1);
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