/*
 * This file implements a dynamic list.
 * Multiple items can be pushed to it and pop again,
 * the size is dynamic, and it allocates memory by itself.
 * */
#include <memory.h>
#include "list.h"

#define LIST_DEFAULT_BUFFER_MUL 10

/*
 * Initialize a list_t structure,
 * and allocate memory for it.
 * */
extern int list_init(list_t* list, size_t item_size) {
    // adjust the item_size to be in bytes
    size_t initial_size = item_size * LIST_DEFAULT_BUFFER_MUL;

    list->item_size = item_size;
    list->real_size = 0;
    list->alloc_size = initial_size;

    // allocate memory for the list
    list->base_ptr = malloc(list->alloc_size);
    if (list->base_ptr == NULL)
        return 1;
    return 0;
}

/*
 * Push an item to the list
 * the item is expected to be the size,
 * that was specified by the creation of this list.
 * */
extern int list_push(list_t* list, void* item) {
    while (list->alloc_size - list->real_size < list->item_size) {
        // allocate double of the current size
        void* ptr = realloc(list->base_ptr, list->alloc_size * 2);

        if (ptr == NULL) return 1;
        list->base_ptr = ptr;
        list->alloc_size *= 2;
    }

    // copy the item
    void* top = list->base_ptr + list->real_size;
    memcpy(top, item, list->item_size);
    list->real_size += list->item_size;
}

/*
 * Pop the last item from the list.
 * If the memory that is used by the queue is lower than the allocated memory,
 * lower the memory so that the allocated memory is double of the used memory.
 * */
extern int list_pop(list_t* list) {
    if (list->real_size - list->item_size == 0)
        return list_clear(list);

    list->real_size -= list->item_size;

    if (list->real_size * 2 < list->alloc_size) {
        void* ptr = realloc(list->base_ptr, list->real_size * 2);

        if (ptr == NULL) return 1;
        list->base_ptr = ptr;
        list->alloc_size = list->real_size * 2;
    }
    return 0;
}

/*
 * Get an item in list.
 * */
extern void* list_get(list_t* list, size_t index) {
    return (void*)((size_t)list->base_ptr + (list->item_size * index));
}

/*
 * Iterate over the list.
 * */
extern void list_iter(list_t* list, iter_fn_t* fn) {
    size_t size = list_size(list);

    for (size_t i = 0; i < size; ++i) {
        fn(list_get(list, i));
    }
}

/*
 * Return the number of items in the list.
 * */
extern size_t list_size(list_t* list) {
    return list->real_size / list->item_size;
}

/*
 * Clear the list of all items,
 * and deallocate down to buffer_size * item_size,
 * */
extern int list_clear(list_t* list) {
    list->real_size = 0;
    size_t new_size = list->item_size * LIST_DEFAULT_BUFFER_MUL;
    void* ptr = realloc(list->base_ptr, new_size);

    if (ptr == NULL) return 1;
    list->base_ptr = ptr;
    list->alloc_size = new_size;
    return 0;
}

/*
 * Free the list memory.
 * */
extern int list_free(list_t* list) {
    free(list->base_ptr);
    list->base_ptr = NULL;
    list->real_size = 0;
    list->alloc_size = 0;
}
