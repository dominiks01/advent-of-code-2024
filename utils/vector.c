#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Create a vector object
 *
 * @param item_size
 * @param initial_capacity
 * @return vector*
 */
vector* create_vector(size_t item_size, size_t initial_capacity) {
    vector* v = malloc(sizeof(vector));
    v->item_size = item_size;
    v->capacity = initial_capacity;
    v->count = 0;

    if (initial_capacity == 0) {
        v->capacity = 1;
    }

    v->items = malloc(item_size * v->capacity);

    if (v->items == NULL) {
        free(v);
        return NULL;
    }

    return v;
}

/**
 * @brief
 *
 * @param v
 * @param value
 */
void vector_push_back(vector* v, void* value) {

    if (v->capacity * 2 > MAX_ALLOWED_SIZE) {
        fprintf(stderr, "Requested size exceeds limit!\n");
        return;
    }

    if (v->count >= v->capacity) {
        size_t new_capacity = v->capacity * 2;
        void* new_items = realloc(v->items, v->item_size * new_capacity);

        if (new_items == NULL) {
            fprintf(stderr, "Memory allocation failed!\n");
            return;
        }

        v->items = new_items;
        v->capacity = new_capacity;
    }

    void* target = ( char* )v->items + (v->count * v->item_size);
    memcpy(target, value, v->item_size);
    v->count++;
}

/**
 * @brief
 *
 * @param v
 * @param index
 * @return void*
 */
void* vector_get(vector* v, size_t index) {
    if (v->count <= index || index < 0) {
        return NULL;
    }

    return ( char* )v->items + v->item_size * index;
}

/**
 * @brief
 *
 * @param v
 */
void vector_free(vector* v) {
    if (v != NULL) {

        if (v->items != NULL) {
            free(v->items);
            v->items = NULL;
        }

        v->count = 0;
        v->capacity = 0;
    }
}

/**
 * @brief
 *
 * @param v
 * @param index
 */
void vector_delete(vector* v, size_t index) {
    if (!v) {
        return;
    }

    void* target = ( void* )(v->items + index * v->item_size);
    if (target == NULL) {
        return;
    }

    size_t move_mem = (v->count - index - 1) * v->item_size;
    if (move_mem > 0) {
        memcpy(target, ( char* )target + v->item_size, move_mem);
    }

    v->capacity--;
}

/**
 * @brief
 *
 * @param v
 * @param item
 * @param index
 */
void vector_insert(vector* v, void* item, size_t index) {
    if (!v || !v->items || !item) {
        return;
    }

    if (index >= v->capacity) {
        return;
    }

    void* target = ( void* )(v->items + index * v->item_size);
    memcpy(target, item, v->item_size);
}

void vector_shallow_copy(vector* dest, vector* src) {
    *dest = *src;
    dest->items = malloc(src->capacity * sizeof(*src->items));

    if (dest->items == NULL) {
        exit(EXIT_FAILURE);
    }

    memcpy(dest->items, src->items, src->capacity * sizeof(*src->items));
}