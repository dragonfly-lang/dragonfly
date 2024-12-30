#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef struct {
    char* source;
    size_t position;
} Lexer;

Lexer* create_lexer(const char* source);
Token lex_number(Lexer* lexer);
Token lex_identifier(Lexer* lexer);
Token lex_symbol(Lexer* lexer);
Token lex_string(Lexer* lexer);
Token is_keyword(Token token);
TokenList* tokenise(const char* source);
void free_lexer(Lexer* lexer);

#endif // LEXER_H