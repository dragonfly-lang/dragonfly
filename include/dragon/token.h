#pragma once

#include <string>
#include <format>
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

    // Keywords
    TOKEN_LET_KW,                   // "let"
    TOKEN_MUT_KW,                   // "mut"
    TOKEN_INT_KW,                   // "int"
    TOKEN_IF_KW,                    // "if"
    TOKEN_ELSE_KW,                  // "else"
    TOKEN_FOR_KW,                   // "for"
    TOKEN_IN_KW,                    // "in"
    TOKEN_FUNC_KW,                  // "func"
    TOKEN_RETURN_KW,                // "return"
    TOKEN_WHILE_KW,                 // "while"
    TOKEN_TRUE_KW,                  // "true"
    TOKEN_FALSE_KW,                 // "false"
    TOKEN_BOOL_KW,                  // "bool"
    TOKEN_BREAK_KW,                 // "break"
    TOKEN_CONTINUE_KW,              // "continue"
    TOKEN_STRUCT_KW,                // "struct"
    TOKEN_ENUM_KW,                  // "enum"
    TOKEN_TYPE_KW,                  // "type"
    TOKEN_MATCH_KW,                 // "match"
    TOKEN_IMPORT_KW,                // "import"
    TOKEN_AS_KW,                    // "as"

    // Literals
    TOKEN_INTEGER,                  // 123
    TOKEN_FLOAT,                    // 123.45
    TOKEN_IDENTIFIER,               // variable_name
    TOKEN_STRING,                   // "string"
    TOKEN_CHAR,                     // 'c'

    // Symbols
    TOKEN_EQUALS,                   // =
    TOKEN_PLUS,                     // +
    TOKEN_MINUS,                    // -
    TOKEN_ASTERISK,                 // *
    TOKEN_SLASH,                    // /
    TOKEN_MODULO,                   // %
    TOKEN_AND,                      // &&
    TOKEN_OR,                       // ||
    TOKEN_NOT,                      // !
    TOKEN_EQUALITY,                 // ==
    TOKEN_NOT_EQ,                   // !=
    TOKEN_GRT,                      // >
    TOKEN_LSS,                      // <
    TOKEN_LTE,                      // <=
    TOKEN_GTE,                      // >=
    TOKEN_LSHIFT,                   // <<
    TOKEN_RSHIFT,                   // >>
    TOKEN_AMPERSAND,                // &
    TOKEN_PIPE,                     // |
    TOKEN_CARET,                    // ^
    TOKEN_TILDE,                    // ~
    TOKEN_BRACE_OPEN,               // {
    TOKEN_BRACE_CLOSE,              // }
    TOKEN_PAREN_OPEN,               // (
    TOKEN_PAREN_CLOSE,              // )
    TOKEN_COMMA,                    // ,
    TOKEN_SEMICOLON,                // ;
    TOKEN_COLON,                    // :
    TOKEN_DOT,                      // .
    TOKEN_RANGE,                    // ..
    TOKEN_DOUBLE_RIGHT_ARROW,       // =>
    TOKEN_RIGHT_ARROW,              // ->

    // Misc
    TOKEN_COMMENT,      // Comment
    TOKEN_EOF,          // End of file
    TOKEN_INVALID       // Invalid token
} TokenType;

static const char* keywords[] = {
    // Variable Declarations
    "let",
    "mut",

    // Data Types
    "int",
    "float",
    "bool",
    "char",

    // Control Flow
    "if",
    "else",
    "for",
    "in",
    "while",
    "break",
    "continue",

    // Boolean Literals
    "true",
    "false",

    // Functions
    "func",
    "return",

    // Modules and Types
    "import",
    "struct",
    "enum",
    "type",
    "match",
    "as"
};


typedef struct {
    TokenType type;
    char* value;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenList;

TokenList* create_token_list();
void append_token(TokenList* list, Token token);
void free_tokens(TokenList* list);

#endif 