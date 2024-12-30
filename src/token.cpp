#include "dragon/token.h"
#include <stdlib.h>
#include <string.h>

TokenList* create_token_list() {
    TokenList* list = (TokenList*)malloc(sizeof(TokenList));
    list->tokens = (Token*)malloc(sizeof(Token) * 8);
    list->count = 0;
    list->capacity = 8;
    return list;
}

void append_token(TokenList* list, Token token) {
    if (list->count >= list->capacity) {
        list->capacity *= 2;
        list->tokens = (Token*)realloc(list->tokens, sizeof(Token) * list->capacity);
    }
    list->tokens[list->count++] = token;
}

void free_tokens(TokenList* list) {
    for (size_t i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
    free(list->tokens);
    free(list);
}
