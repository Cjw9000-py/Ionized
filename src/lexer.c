
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <assert.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#include "list.h"
#include "token.h"
#include "lexer.h"



size_t lexer_parse_token_defs(const char* text, size_t length, token_def_t** buffer) {
    // count the line numbers
    size_t line_count = 0;
    for (size_t i = 0; i < length; ++i) {
        if (text[i] == '\n')
            line_count++;
    }

    struct line_t {
        const char* ptr;
        size_t length;
    } lines[line_count];
    
    // search for all the strings
    const char* ptr = text;
    for (size_t i = 0; i < line_count; ++i) {
        char* idx = index(ptr, '\n');
        assert(idx != NULL);
        
        lines[i].ptr = ptr;
        lines[i].length = idx - ptr;
        
        ptr = idx + 1;
    }
    
    // allocate own memory and terminate the lines
    struct line_t terminated_lines[line_count];

    for (size_t i = 0; i < line_count; ++i) {
        char* p = malloc(lines[i].length + 1);
        if (p == NULL) return 1;
        memcpy(p, lines[i].ptr, lines[i].length);

        terminated_lines[i].ptr = p;
        terminated_lines[i].length = lines[i].length + 1;
        
        // terminate
        p[lines[i].length] = 0;
    }

    token_def_t entries_stack[line_count];
    size_t consumed = 0;

    // parse each line
    for (size_t i = 0; i < line_count; ++i) {
        // check if the line is empty
        if (terminated_lines[i].length == 1)
            continue;

        // read the first number
        char* end;

        errno = 0;
        long num = strtol(terminated_lines[i].ptr, &end, 10);

        if (errno != 0) {
            // an error happened
            fprintf(stderr, "could not read number on line %i", i);
            perror("error");
            return 1;
        }

        if (end[0] != ':') {
            fprintf(stderr, "expected ':' after the first value on line %i", i);
            return 1;
        }
        end++;

        // extract the name
        char* idx = index(end, '=');
        if (idx == NULL) {
            fprintf(stderr, "expected '=' on line %i", i);
            return 1;
        }

        size_t name_size = idx - end + 1;
        char* name = malloc(name_size);

        memcpy(name, end, name_size);
        name[name_size - 1] = 0;

        // capture the rest
        void* end_ptr = (void*)(terminated_lines[i].ptr + terminated_lines[i].length);
        size_t remaining = end_ptr - (void*)idx + 1;

        char* pattern = malloc(remaining);
        memcpy(pattern, idx + 1, remaining);
        pattern[remaining - 1] = 0;

        entries_stack[i].type = num;
        entries_stack[i].name = name;
        entries_stack[i].pattern = pattern;
        consumed++;

        // list_push(&entries, &local);
    }

    // move all items to the buffer
    *buffer = malloc(consumed);
    token_def_t* buffer_ptr = *buffer;

    for (size_t i = 0; i < consumed; ++i) {
        void* dest = buffer_ptr + i;
        void* p = &entries_stack[i];

        memcpy(dest, p, sizeof(token_def_t));
    }

    // free terminated lines
    for (size_t i = 0; i < line_count; ++i) {
        free((char*)terminated_lines[i].ptr);
    }

    return consumed;
}

/*
 * Load all token definitions from file.
 * */
size_t lexer_load_token_defs(lexer_t* lexer, const char* file_name) {
    // open the file
    FILE* fd = fopen(file_name, "rb");
    if (fd == NULL) return 0;

    // stat for size
    struct stat st;
    if (stat(file_name, &st)) {
        LOG_ERROR("could not open token file");
        perror(file_name);
        fclose(fd);
        return 0;
    }

    if (st.st_size == 0) {
        return 0;
    }

    // load the file into memory
    char* map = malloc(st.st_size);
    fread(map, 1, st.st_size, fd);
    fclose(fd);

    size_t ret = lexer_parse_token_defs(map, st.st_size, &lexer->token_defs);

    free(map);
    return ret;
}

/*
 * Compile the regex pattern inside each token def
 * */
int lexer_compile_all_token_defs(lexer_t* lexer) {
    for (size_t i = 0; i < lexer->token_def_count; ++i) {
        int res = regcomp(
            &lexer->token_defs[i].re_comp,
            lexer->token_defs[i].pattern,
            0
        );

        if (res != 0) {
            // an error occurred
            fprintf(stderr, "Error while compiling regex pattern for token %s", lexer->token_defs[i].name);
            switch (res) {
                case REG_BADBR: LOG_ERROR("Invalid use of back reference operator."); break;
                case REG_BADPAT: LOG_ERROR("Invalid use of pattern operators such as group or list."); break;
                case REG_BADRPT: LOG_ERROR("Invalid use of repetition operators such as using '*' as the first character."); break;
                case REG_EBRACE: LOG_ERROR("Un-matched brace interval operators."); break;
                case REG_EBRACK: LOG_ERROR("Un-matched bracket list operators."); break;
                case REG_ECOLLATE: LOG_ERROR("Invalid collating element."); break;
                case REG_ECTYPE: LOG_ERROR("Unknown character class name."); break;
                case REG_EEND: LOG_ERROR("Nonspecific error.  This is not defined by POSIX.2."); break;
                case REG_EESCAPE: LOG_ERROR("Trailing backslash."); break;
                case REG_EPAREN: LOG_ERROR("Un-matched parenthesis group operators."); break;
                case REG_ERANGE: LOG_ERROR("Invalid use of the range operator"); break;
                case REG_ESIZE: LOG_ERROR("Compiled regular expression requires a pattern buffer larger than 64 kB. This is not defined by POSIX.2."); break;
                case REG_ESPACE: LOG_ERROR("The regex routines ran out of memory."); break;
                case REG_ESUBREG: LOG_ERROR("Invalid back reference to a subexpression."); break;
                default: LOG_ERROR("Unknown Error.");
            }

            return 1;
        }
    }
    return 0;
}


void lexer_free_token_defs(token_def_t * token_defs, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        free(token_defs[i].name);
        free(token_defs[i].pattern);
    }
}

void lexer_free(lexer_t* lex_data) {
    lexer_free_token_defs(lex_data->token_defs, lex_data->token_def_count);
}
