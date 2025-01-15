#include "vector.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

vector* create_vector(size_t item_size, size_t initial_capacity) {
    vector* v = malloc(sizeof(vector));

    v->item_size = item_size;
    v->capacity = initial_capacity;
    v->count = 0;
    v->items = malloc(item_size * initial_capacity);

    return v;
}

void push_back(vector* v, void* value) {
    if (v->count >= v->capacity) {
        v->capacity *= 2;
        v->items = realloc(v->items, v->item_size * v->capacity);
    }

    void* target = ( char* )v->items + (v->count * v->item_size);
    memcpy(target, value, v->item_size);
    v->count++;
}

void* get(vector* v, size_t index) {
    if (v->count <= index || index < 0) {
        printf("index out-of-range!\n");
        return NULL;
    }

    return ( char* )v->items + v->item_size * index;
}

void free_vector(vector* v) {
    free(v->items);
    v->items = NULL;
    v->count = 0;
    v->capacity = 0;
}

void emplace_vector(vector* v, void* item, size_t index) {
    if (!v) {
        return;// NULL ptr dereference
    }

    if (!item) {
        return;
    }

    if (v->items + index * v->item_size == NULL) {
        return;
    }

    void** target = ( void** )(v->items + index * v->item_size);
    *target = item;
}

void print_vector(vector* v) {

    if (!v) {
        return;// NULL ptr dereference
    }

    printf("vector: [");

    for (size_t i = 0; v->capacity - 1; i++) {
        printf("%s, ", ( char* )(get(v, v->item_size)));
    }

    printf("%s]\n", ( char* )(get(v, v->count - 1)));
}