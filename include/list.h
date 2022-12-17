/*
 * This file implements a dynamic list.
 * Multiple items can be pushed to it and pop again,
 * the size is dynamic, and it allocates memory by itself.
 * */


#pragma once
#include <stdlib.h>


struct List {
    // the base ptr points to the beginning of the memory page
    void* base_ptr;
    // the top ptr points to the top of the list
    size_t real_size;
    // the alloc ptr points to the last byte that is allocated
    size_t alloc_size;
    size_t item_size;
};

typedef struct List list_t;
typedef void iter_fn_t(void*);

extern int list_init(list_t* list, size_t item_size);
extern int list_push(list_t* list, void* item);
extern int list_pop(list_t* list);
extern void* list_get(list_t* list, size_t index);
extern void list_iter(list_t* list, iter_fn_t*);
extern size_t list_size(list_t* list);
extern int list_clear(list_t* list);
extern int list_free(list_t* list);