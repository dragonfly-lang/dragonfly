#include "dragon/lexer.h"

Lexer* create_lexer(const char* source) {
    Lexer* lexer = (Lexer *)malloc(sizeof(Lexer));
    lexer->source = strdup(source);
    lexer->position = 0;
    return lexer;
}

void free_lexer(Lexer* lexer) {
    free(lexer->source);
    free(lexer);
}

TokenList* tokenise(const char* source) {
    TokenList* tokens = create_token_list();

    if (source == NULL || strlen(source) == 0) {
        return tokens;
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

Token lex_string(Lexer* lexer) {
    char quote = lexer->source[lexer->position];
    lexer->position++;

    size_t start = lexer->position;
    while (lexer->source[lexer->position] != quote) {
        lexer->position++;
    }

    size_t length = lexer->position - start;
    char* value = strndup(lexer->source + start, length);

    lexer->position++;

    Token token = {TOKEN_STRING, value};
    return token;
}

Token is_keyword(Token token) {
    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(token.value, keywords[i]) == 0) {
            token.type = (TokenType)i;  
            return token;
        }
    }
    return token;  
}
