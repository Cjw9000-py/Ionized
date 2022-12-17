#pragma once

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"

#define MALLOC_TESTS(c) test_entry_t* ptr = malloc(sizeof(test_entry_t) * c); *tests = ptr;
#define TEST_ENTRY(c, func) ptr[c].name = #func; ptr[c].fn = &func;


typedef int test_function_t();

struct TestEntry {
    const char* name;
    test_function_t* fn;
};

typedef struct TestEntry test_entry_t;


size_t expose_tests(test_entry_t** tests);
test_entry_t* get_tests(int amount) {
    assert(amount != 0);
    return malloc(sizeof(test_entry_t) * amount);
}


int main() {
    test_entry_t* tests;
    size_t count = expose_tests(&tests);

    int code = 0;
    for (int i = 0; i < count; ++i) {
        test_entry_t* entry = (tests + i);
        assert(entry->name != NULL);
        assert(entry->fn != NULL);

        printf("Running test %s\n", entry->name);
        int res = entry->fn();
        if (res == 0)
            printf("Test %s succeeded.\n", entry->name);
        else {
            printf("Test %s has failed with return code %i\n", entry->name, res);
            code = 1;
        }

    }

    free(tests);
    return code;
}
