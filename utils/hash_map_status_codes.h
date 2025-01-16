// hash_map_status_codes.h

#ifndef HASH_MAP_STATUS_CODES_H
#define HASH_MAP_STATUS_CODES_H

typedef enum {
    HASH_MAP_SUCCESS = 0,
    HASH_MAP_ERROR_INVALID_KEY = 1,
    HASH_MAP_ERROR_KEY_NOT_FOUND = 2,
    HASH_MAP_ERROR_NULL_PTR = 3,
    HASH_MAP_ERROR_OUT_OF_MEMORY = 4
} HashMapStatusCode;

#endif