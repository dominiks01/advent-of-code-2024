#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <complex.h>
#include <stdint.h>
#include <stdio.h>

typedef struct Node {
    char* key;
    void* item;
    struct Node* next;
} Node;

typedef struct {
    Node** table;
    size_t item_size;
    size_t capacity;
} HashMap;

uint32_t hash(const void*, size_t);
HashMap* create_hash_map(const int, size_t);
void* search_map(HashMap*, const char*);
void insert(HashMap*, const char*, void*);
void free_hash_map(HashMap*);
int delete_from_map(HashMap*, const char*);

#endif