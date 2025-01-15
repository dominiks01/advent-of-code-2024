#ifndef VECTOR_H
#define VECTOR_H

#include <complex.h>
#include <stdio.h>

typedef struct {
    size_t item_size;
    void* items;
    size_t capacity;
    size_t count;
} vector;

vector* create_vector(size_t, size_t);
void push_back(vector*, void*);
void* get(vector*, size_t);
void free_vector(vector*);
void print_vector(vector*);
void emplace_vector(vector*, void*, size_t);

#endif