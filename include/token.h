#pragma once

#include <inttypes.h>
#include <regex.h>


enum TokenType {

    // special
    TOK_EOF,  // special
    TOK_NEWLINE,  // \n

    // comments
    TOK_LINE_COMMENT,
    TOK_COMMENT,  // /* ... */

    TOK_DOT,  // .
    TOK_DDOT,  // ..
    TOK_ELLIPSIS,  // ...
    TOK_COMMA,  // ,
    TOK_COLON,  // :
    TOK_SEMICOLON,  // ;
    TOK_QUEST,  // ? (not used atm)
    TOK_AT,  // @

    // literals
    TOK_NUMBER,
    TOK_STRING,
    TOK_TRUE,
    TOK_FALSE,
    TOK_HEX_NUM,
    TOK_BIN_NUM,
    TOK_DECIMAL,
    TOK_NULL,

    // bin op
    TOK_ADD,  // +
    TOK_SUB,  // -
    TOK_MUL,  // *
    TOK_DIV,  // /
    TOK_XOR,  // ^
    TOK_NEG,  // ~
    TOK_MOD,  // %
    TOK_BOR,  // |
    TOK_BAND,  // &
    TOK_LSHIFT,  // <<
    TOK_RSHIFT,  // >>
    TOK_POW,  // **
    TOK_FLOORDIV,  // //
    TOK_INC,  // ++
    TOK_DEC,  // --

    // bool operators
    TOK_EQ,  // ==
    TOK_NE,  // !=
    TOK_LT,  // <
    TOK_GT,  // >
    TOK_LE,  // <=
    TOK_GE,  // >=
    TOK_NOT,  // not
    TOK_OR,  // or
    TOK_AND,  // and

    // parent
    TOK_LPAREN,  // (
    TOK_RPAREN,  // )
    TOK_LBRACE,  // [
    TOK_RBRACE,  // ]
    TOK_LBRACKET,  // {
    TOK_RBRACKET,  // }

    // assign
    TOK_ASSIGN,  // =
    TOK_ADD_ASSIGN,  // +=
    TOK_SUB_ASSIGN,  // -=
    TOK_MUL_ASSIGN,  // *=
    TOK_DIV_ASSIGN,  // /=
    TOK_NEG_ASSIGN,  // ~=
    TOK_MOD_ASSIGN,  // %=
    TOK_POW_ASSIGN,  // **=
    TOK_FLOORDIV_ASSIGN,  // //=
    TOK_BOR_ASSIGN,  // |=
    TOK_BAND_ASSIGN,  // &=
    TOK_XOR_ASSIGN,  // ^=
    TOK_LSHIFT_ASSIGN,  // <<=
    TOK_RSHIFT_ASSIGN,  // >>=

    // keyword
    TOK_IF,
    TOK_ELIF,
    TOK_ELSE,
    TOK_WHILE,
    TOK_FOR,
    TOK_NEW,
    TOK_FROM,
    TOK_IN,
    TOK_IS,
    TOK_AS,
    TOK_WITH,
    TOK_BREAK,
    TOK_CONTINUE,
    TOK_RETURN,
    TOK_DO,
    TOK_TRY,
    TOK_EXCEPT,
    TOK_FINALLY,
    TOK_RAISE,
    TOK_SWITCH,
    TOK_CASE,
    TOK_DEFAULT,
    TOK_STRUCT,
    TOK_CLASS,
    TOK_ENUM,
    TOK_CONST,
    TOK_STATIC,
    TOK_ABSTRACT,

    TOK_IDENT,
};

//int tok_enum_span[2] = {TOK_EOF, TOK_IDENT};
//size_t tok_type_start = TOK_EOF;
//siz

struct TokenDefinition {
    uint16_t type;
    char* name;
    char* pattern;
    regex_t re_comp;
};

typedef struct TokenDefinition token_def_t;

size_t get_token_defs(struct TokenDefinition**);