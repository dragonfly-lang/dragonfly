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

    Lexer* lexer = create_lexer(source);

    while (lexer->position < strlen(lexer->source)) {
        char c = lexer->source[lexer->position];

        while (c == ' ' || c == '\n' || c == '\t') {
            lexer->position++;
            c = lexer->source[lexer->position];
        }

        Token token = {TOKEN_INVALID, NULL};
        if (isdigit(c)) {
            token = lex_number(lexer);
        } else if (isalpha(c)) {
            token = lex_identifier(lexer);
        } else if (c == '"' || c == '\'') {
            token = lex_string(lexer);
        } else {
            token = lex_symbol(lexer);
        }

        append_token(tokens, token);
    }

    free_lexer(lexer);

    return tokens;
}

Token lex_number(Lexer* lexer) {
    size_t start = lexer->position;
    while (isdigit(lexer->source[lexer->position])) {
        lexer->position++;
    }

    size_t length = lexer->position - start;
    char* value = strndup(lexer->source + start, length);

    Token token = {TOKEN_INTEGER, value};
    return token;
}

Token lex_identifier(Lexer* lexer) {
    size_t start = lexer->position;
    while (isalnum(lexer->source[lexer->position])) {
        lexer->position++;
    }

    size_t length = lexer->position - start;
    char* value = strndup(lexer->source + start, length);

    Token token = {TOKEN_IDENTIFIER, value};

    token = is_keyword(token);

    return token;
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