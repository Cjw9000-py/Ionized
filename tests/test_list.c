#include <stdio.h>
#include <assert.h>
#include <inttypes.h>
#include "list.h"

#include "test_utils.h"


struct Vec3 {
    uint64_t x, y, z;
};

typedef struct Vec3 vec3;


int test_push() {
    list_t* x = malloc(sizeof(list_t));
    if (x == NULL) return 1;

    vec3* v = malloc(sizeof(vec3));
    v->x = 42;
    v->y = 42 * 2;
    v->z = 42 * 3;

    list_init(x, sizeof(vec3));

    for (int i = 0; i < 100; i++) {
        list_push(x, v);
    }

    assert(x->real_size == sizeof(vec3) * 100);
    assert(x->alloc_size > sizeof(vec3) * 100);

    for (int i = 0; i < 100; i++) {
        list_pop(x);
    }

    assert(list_size(x) == 0);

    list_free(x);
    free(x);
    free(v);
}

int test_get() {
    list_t* x = malloc(sizeof(list_t));
    if (x == NULL) return 1;

    uint64_t* v = malloc(sizeof(uint64_t));
    *v = 42;

    list_init(x, sizeof(uint64_t));

    for (int i = 0; i < 100; ++i) {
        list_push(x, v);
    }

    for (int i = 0; i < 100; ++i) {
        uint64_t item = *(uint64_t*)list_get(x, i);
        assert(item == 42);
    }

    list_free(x);
    free(x);
    free(v);
}

int test_clear() {
    list_t* x = malloc(sizeof(list_t));
    if (x == NULL) return 1;

    uint64_t* v = malloc(sizeof(uint64_t));
    *v = 42;

    list_init(x, sizeof(uint64_t));

    for (int i = 0; i < 100; ++i) {
        list_push(x, v);
    }

    list_clear(x);
    assert(list_size(x) == 0);

    list_free(x);
    free(x);
    free(v);
}

size_t expose_tests(test_entry_t** tests) {
    MALLOC_TESTS(3);

    TEST_ENTRY(0, test_push);
     TEST_ENTRY(1, test_get);
     TEST_ENTRY(2, test_clear);

    return 3;
}