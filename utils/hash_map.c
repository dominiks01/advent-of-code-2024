#include "hash_map.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map_status_codes.h"

#define FNV_OFFSET_BASIS 0x811C9DC5
#define FNV_PRIME 0x01000193
#define HASH_MAP_DEFAULT_CAPACITY 10
#define HASH_MAP_MEMORY_RESIZE_SCALAR 2
#define MAX_ALLOWED_SIZE 65536

/**
 *  @brief Calculate hash value of key using FNV-1a algorithm
 *
 *  @param key HashMap key
 *  @param len HashMap capacity for hash calculation
 *  @return uint32_t Calculated hash value
 */
static uint32_t hash_map_hash_function(const char *key, const uint32_t len) {

    if (key == NULL || len <= 0) {
        return 0;// Invalid input
    }

    uint32_t hash = FNV_OFFSET_BASIS;// FNV Offset Basis
    const uint8_t *bytes = ( const uint8_t * )key;

    while (*bytes) {
        hash = hash ^ ( uint32_t )(*bytes);// XOR operation
        hash = hash * FNV_PRIME;           // FNV Prime
        bytes++;
    }
    return hash % ( uint32_t )len;
}

/**
 * @brief Create and initialize HashMap
 * Only one argument needed is size of stored object.
 * Default HashMap capacity (HASH_MAP_DEFAULT_CAPACITY) is set to 10.
 *
 * @param item_size Size of object to be mapped.
 * @return HashMap* Pointer to new HashMap
 */
HashMap *hash_map_create() {

    HashMap *hash_map = ( HashMap * )malloc(sizeof(HashMap));
    if (!hash_map) {
        return NULL;
    }

    hash_map->capacity = HASH_MAP_DEFAULT_CAPACITY;
    hash_map->count = 0;
    hash_map->table = ( Node ** )malloc(hash_map->capacity * sizeof(Node *));

    if (!hash_map->table) {
        free(hash_map);
        return NULL;// Memory allocation failed
    }

    for (size_t i = 0; i < hash_map->capacity; i++) {
        hash_map->table[i] = NULL;
    }

    hash_map->capacity = hash_map->capacity;
    return hash_map;
}

/**
 * @brief Resize map.
 *
 * Function takes HashMap as argument and then resize table used for storing
 * pointers to nodes with new size equal to:
 *
 *      NEW_CAPACITY = MAP->CAPACITY * HASH_MAP_MEMORY_RESIZE_SCALAR
 *
 * Then function recalculate new keys of all elements in table and
 * insert them in new table.
 *
 * @param map Map to resize
 * @return HashMapStatusCode
 */
static HashMapStatusCode resize_map(HashMap *map) {
    size_t new_capacity = HASH_MAP_MEMORY_RESIZE_SCALAR * map->capacity;

    // Allocate space for new table.
    Node **new_table = calloc(new_capacity, sizeof(Node *));

    if (!new_table) {
        return HASH_MAP_ERROR_OUT_OF_MEMORY;
    }

    // Initialize values of table to null
    for (size_t i = 0; i < new_capacity; i++) {
        new_table[i] = NULL;
    }

    // Recalculate hash for all keys.
    for (size_t i = 0; i < map->capacity; i++) {
        Node *curr = map->table[i];

        while (curr != NULL) {
            Node *next = curr->next;

            uint32_t key_hash_value =
                hash_map_hash_function(curr->key, new_capacity);

            curr->next = new_table[key_hash_value];
            new_table[key_hash_value] = curr;

            curr = next;
        }
    }

    free(map->table);
    map->table = new_table;
    map->capacity = new_capacity;

    return HASH_MAP_SUCCESS;
}

/**
 *  @brief Insert into HashMap object under given key.
 *
 *  @param map HashMap to store value.
 *  @param key Key for accessing object.
 *  @param object Object to insert into map under `key`.
 */
HashMapStatusCode hash_map_insert(HashMap *map, const char *key, void *object,
                                  size_t object_size) {

    // Resize Hash Map if there is not enought space
    if (map->count >= map->capacity) {
        HashMapStatusCode status = resize_map(map);
        if (status != HASH_MAP_SUCCESS) return status;
    }

    // Calculate hash value for new key
    uint32_t key_hash_value = hash_map_hash_function(key, map->capacity);

    // Overwrite object if already in map.
    // This could be changed to duplication/rejection of inserion etc.
    Node *curr = map->table[key_hash_value];

    while (curr) {
        if (strcmp(curr->key, key) == 0) {

            if (object_size >= MAX_ALLOWED_SIZE) {
                fprintf(stderr, "Requested size exceeds limit!\n");
                return HASH_MAP_ERROR_OUT_OF_MEMORY;
            }

            void *new_item = malloc(object_size);
            if (!new_item) {
                return HASH_MAP_ERROR_OUT_OF_MEMORY;
            }
            memcpy(new_item, object, object_size);
            free(object);
            // free(curr->item);
            curr->item = new_item;

            return HASH_MAP_SUCCESS;
        }
        curr = curr->next;
    }

    // Allocate new node
    Node *new_node = ( Node * )malloc(sizeof(Node));
    if (new_node == NULL) {
        return HASH_MAP_ERROR_OUT_OF_MEMORY;// Memory allocation failed
    }

    new_node->item = malloc(object_size);
    new_node->key = strdup(key);
    new_node->node_size = object_size;

    memcpy(new_node->item, object, new_node->node_size);
    free(object);

    // Add newly created node to HashMap
    new_node->next = map->table[key_hash_value];
    map->table[key_hash_value] = new_node;
    map->count++;

    return HASH_MAP_SUCCESS;
}

/**
 *  @brief Search object in HashMap with given key.
 *
 *  @param map HashMap on which search operation will be performed.
 *  @param key Key of searched object
 *  @return void* Pointer to object stored in HashMap or NULL
 */
void *hash_map_find(HashMap *map, const char *key) {
    uint32_t index =
        hash_map_hash_function(key, map->capacity);// hash calculation
    Node *current = map->table[index];

    // Search linked list if not found under 'key'.
    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->item;
        } else {
            current = current->next;
        }
    }

    return NULL;// Object with given key not found
}

/**
 * @brief Delete object with given key.
 *
 * @param map HashMap from which object should be deleted
 * @param key Key of object to delete
 * @return HashMapStatusCode Status code of operation
 */
HashMapStatusCode hash_map_delete(HashMap *map, const char *key) {
    uint32_t index = hash_map_hash_function(key, map->capacity);

    Node *current = map->table[index];
    Node *prev = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                map->table[index] = current->next;
            }

            free(current->item);
            free(current->key);
            free(current);
            return HASH_MAP_SUCCESS;
        }
        prev = current;
        current = current->next;
    }

    return HASH_MAP_ERROR_INVALID_KEY;
}

/**
 * @brief Clear all memory used for HashMap
 *
 * @param map HashMap to free
 * @return HashMapStatusCode Status code of operation
 */
HashMapStatusCode hash_map_free(HashMap *map) {

    if (!map) {
        return HASH_MAP_ERROR_NULL_PTR;// NULL ptr dereference
    }

    for (size_t i = 0; i < map->capacity; i++) {
        Node *current = map->table[i];

        while (current) {
            Node *temp = current;
            current = current->next;

            if (temp->key != NULL) {
                free(temp->key);
            }

            if (temp->item != NULL) {
                free(temp->item);
            }

            if (temp != NULL) {
                free(temp);
            }
        }
    }
    free(map->table);

    return HASH_MAP_SUCCESS;
}
