#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../utils/graph.h"
#include "./../utils/hash_map.h"
#include "./../utils/vector.h"

int main(int argc, char** argv) {

    char line[256];
    const char* filename = "input.txt";
    int first = 0, second = 0, value = 0, index = 0;
    char str[5];

    int result = 0;

    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("fopen() failed with file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    vector* v = create_vector(sizeof(vector*), 100);
    free_vector(v);

    char key_str[255];
    HashMap* map = hash_map_create(sizeof(vector));

    for (int i = 1; i < 100; i++) {
        vector* v = create_vector(sizeof(vector), 2);
        snprintf(key_str, 255, "key_string_%i", i);

        for (int j = 0; j < i; j++) {
            int value = ( int )(i * 10 + j);
            push_back(v, &value);
        }

        hash_map_insert(map, key_str, v,
                        sizeof(vector) + v->capacity * v->item_size);
    }

    for (int i = 1; i < 100; i++) {
        snprintf(key_str, 255, "key_string_%i", i);
        vector* act = hash_map_find(map, key_str);

        printf("vector: [");

        for (size_t j = 0; j < act->count - 1; j++) {
            int* value = ( int* )get(act, j);

            if (value == NULL) {
                continue;
            }

            printf("%i,", *value);
        }

        int* value = ( int* )get(act, act->count - 1);

        if (value == NULL) {
            printf("]\n");
        } else {
            printf("%i]\n", *( int* )get(act, act->count - 1));
        }
    }

    hash_map_free(map);
    printf("Task 01: result [%i]\n", result);

    return 0;
}