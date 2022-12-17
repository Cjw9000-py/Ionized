
#include <stdio.h>
#include <sys/stat.h>
#include "config.h"
#include "lexer.h"
#include "test_utils.h"


int test_lexer_load_token_defs() {
    lexer_t lexer;

    size_t size = lexer_load_token_defs(&lexer, DATA_TOKEN_FILE);
    assert(size != 0);
    token_def_t* buffer = lexer.token_defs;

    for (int i = 0; i < size; ++i) {
        assert(buffer[i].name != NULL);
        assert(buffer[i].pattern != NULL);
    }

    lexer_free_token_defs(buffer, size);
    return 0;
}

int test_lexer_compile_token_defs() {
    lexer_t lexer;

    FILE* fd = fopen(DATA_TOKEN_FILE, "rb");
    if (fd == NULL) return 0;

    // stat for size
    struct stat st;
    if (fstat(fd->_fileno, &st)) {
        LOG_ERROR("could not open token file");
        perror(DATA_TOKEN_FILE);
        fclose(fd);
        return 0;
    }

    // load the file into memory
    char* map = malloc(st.st_size);
    fread(map, 1, st.st_size, fd);
    fclose(fd);

    assert(lexer_compile_all_token_defs(&lexer));

    lexer_free_token_defs(lexer.token_defs, size);
    return 0;
}




size_t expose_tests(test_entry_t** tests) {
    MALLOC_TESTS(2);

    TEST_ENTRY(0, test_lexer_load_token_defs);
    TEST_ENTRY(1, test_lexer_load_token_defs);

    return 2;
}