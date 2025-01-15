#include "hash_map.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
    FNV-1a hash fucntion.
    https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
*/
uint32_t hash(const void* data, size_t len) {
    const uint8_t* bytes = ( const uint8_t* )data;
    uint32_t hash = 0x811C9DC5;// FNV Offset Basis

    for (size_t i = 0; i < len; ++i) {
        hash = hash ^ bytes[i];
        hash = hash * 0x01000193;// FNV Prime
    }
    return hash;
}

/*
    Create Hash Map with initial capacity.

    Allocate all necessary memory & initialize
    Nodes with NULL values.
*/
HashMap* create_hash_map(const int capacity, size_t item_size) {
    HashMap* hash_map = ( HashMap* )malloc(sizeof(HashMap));
    if (!hash_map) {
        return NULL;// Memory allocation failed
    }

    hash_map->table = ( Node** )malloc(capacity * sizeof(Node*));
    if (!hash_map->table) {
        return NULL;// Memory allocation for table failed
    }

    for (size_t i = 0; i < capacity; i++) {
        hash_map->table[i] = NULL;
    }

    hash_map->item_size = item_size;
    hash_map->capacity = capacity;
    return hash_map;
}

/*
    Insert function
*/
void insert(HashMap* map, const char* key, void* value) {

    // Calculate hash value for new key
    uint32_t key_hash_value = hash(key, map->item_size);

    Node* new_node = ( Node* )malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->item = value;

    new_node->next = map->table[key_hash_value];
    map->table[key_hash_value] = new_node;
}

/*
    Search function
*/
void* search_map(HashMap* map, const char* key) {
    uint32_t index = hash(key, map->item_size);
    Node* current = map->table[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->item;
        } else {
            current = current->next;
        }
    }

    return NULL;
}

/*
    Deletion function
*/
int delete_from_map(HashMap* map, const char* key) {
    uint32_t index = hash(key, map->item_size);

    Node* current = map->table[index];
    Node* prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                map->table[index] = current->next;
            }

            free(current->key);
            free(current);
            return 0;
        }
        prev = current;
        current = current->next;
    }

    return -1;
}

/*
    free-ing all allocated resources
*/
void free_hash_map(HashMap* map) {

    if (!map) {
        return;// NULL ptr dereference
    }

    for (int i = 0; i < map->capacity; i++) {
        Node* current = map->table[i];

        while (current) {
            Node* temp = current;
            current = current->next;

            free(temp->key);
            free(temp->item);
            free(temp);
        }
    }
    free(map);
    free(map->table);
}
