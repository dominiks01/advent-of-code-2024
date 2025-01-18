#ifndef VECTOR_H
#define VECTOR_H

#include <complex.h>
#include <stdio.h>

#define VECTOR_DEFAULT_CAPACITY 10
#define MAX_ALLOWED_SIZE 65536

// Macro to dynamically select the correct overload of `vector_create`
#define vector_create(...)                               \
    create_vector_overload(__VA_ARGS__, vector_create_2, \
                           vector_create_1)(__VA_ARGS__)
#define create_vector_overload(_1, _2, NAME, ...) NAME

#define vector_create_1(size) create_vector(size, VECTOR_DEFAULT_CAPACITY)
#define vector_create_2(size, capacity) create_vector(size, capacity)

typedef struct {
    size_t item_size;
    void* items;
    size_t capacity;
    size_t count;
} vector;

/**
 * @brief Create a vector object
 *
 * @return vector*
 */
vector* create_vector(size_t, size_t);

/**
 * @brief
 *
 */
void vector_push_back(vector*, void*);

/**

*/
void vector_free(vector*);

/**
 * @brief
 *
 * @return void*
 */
void* vector_get(vector*, size_t);

/**
 * @brief
 *
 */
void vector_insert(vector*, void*, size_t);

/**
 * @brief
 *
 */
void vector_delete(vector*, size_t);

void vector_shallow_copy(vector* dest, vector* src);

#endif