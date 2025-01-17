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
 * @brief Create and initialize HashMap
 * Only one argument needed is size of stored object.
 * Default HashMap capacity (HASH_MAP_DEFAULT_CAPACITY) is set to 10.
 *
 * @param item_size Size of object to be mapped.
 * @return HashMap* Pointer to new HashMap
 */
HashMap* hash_map_create();

/**
 *  @brief Insert into HashMap object under given key.
 *
 *  @param map HashMap to store value.
 *  @param key Key for accessing object.
 *  @param object Object to insert into map under `key`.
 */
HashMapStatusCode hash_map_insert(HashMap* map, const char* key, void* object,
                                  size_t object_size);

/**
 *  @brief Search object in HashMap with given key.
 *
 *  @param map HashMap on which search operation will be performed.
 *  @param key Key of searched object
 *  @return void* Pointer to object stored in HashMap or NULL
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