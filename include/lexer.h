#pragma once

#include "token.h"

struct LexerData {
    size_t token_def_count;
    token_def_t* token_defs;
};

typedef struct LexerData lexer_t;

size_t lexer_parse_token_defs(const char* text, size_t length, token_def_t** buffer);
size_t lexer_load_token_defs(lexer_t* lexer, const char* file_name);
int lexer_compile_all_token_defs(lexer_t* lexer);
void lexer_free_token_defs(token_def_t* token_defs, size_t size);
void lexer_free(lexer_t* lex_data);