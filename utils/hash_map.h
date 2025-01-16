#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <complex.h>
#include <stdint.h>
#include <stdio.h>

#include "./hash_map_status_codes.h"

typedef struct Node {
    char* key;
    void* item;
    size_t node_size;
    struct Node* next;
} Node;

typedef struct {
    Node** table;
    size_t capacity;
    size_t count;
} HashMap;

/**
 *  @brief Calculate hash value of key using FNV hash algorithm in version 1a
 *
 *  @param key Key value
 *  @param len HashMap capacity for hash calculation
 *  @return Calculated hash value
 */
uint32_t hash_map_hash_function(const char* key, const uint32_t len);

/**
 * @brief Create HashMap given capacity and size of mapped object type.
 *
 * @param item_size Size of object to be mapped.
 * @return Pointer to newly created HashMap or NULL if operation fails.
 */
HashMap* hash_map_create(size_t item_size);

/**
 *  @brief Insert into HashMap object under given key.
 *
 *  @param map HashMap to store value under key.
 *  @param key Key used for accessing object.
 *  @param object Object to store under key.
 */
HashMapStatusCode hash_map_insert(HashMap* map, const char* key, void* object,
                                  size_t object_size);

/**
 *  @brief Search object in HashMap under given key.
 *
 *  @param map HashMap on which search operation will be performed.
 *  @param key Key of searched object
 *  @return Pointer to object stored in HashMap or NULL
 */
void* hash_map_find(HashMap*, const char*);

/**
 * @brief Clear all memory used for HashMap
 *
 * @param map HashMap to free
 * @return HashMapStatusCode Status code of operation
 */
HashMapStatusCode hash_map_free(HashMap*);

/**
 * @brief Delete object with given key.
 *
 * @param map HashMap from which object should be deleted
 * @param key Key of object to delete
 * @return HashMapStatusCode Status code of operation
 */
HashMapStatusCode hash_map_delete(HashMap*, const char*);

#endif