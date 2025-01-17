#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../utils/hash_map.h"
#include "./../utils/pair.h"
#include "./../utils/vector.h"

HashMap* rules_ordering;

int sort_function(const void* a, const void* b) {

    char str[5];
    snprintf(str, 5, "%i", *( int* )a);

    if (b == NULL || a == NULL) {
        return 0;
    }

    vector* vec = ( vector* )hash_map_find(rules_ordering, str);
    if (vec != NULL) {
        for (size_t i = 0; i < vec->capacity; i++) {
            int b_val = *( int* )b;
            int* rule_value = ( int* )vector_get(vec, i);

            if (rule_value == NULL) {
                continue;
            }

            if (b_val == *rule_value) {
                return -1;
            }
        }
    }

    snprintf(str, 5, "%i", *( int* )b);
    vec = ( vector* )hash_map_find(rules_ordering, str);
    if (vec != NULL) {
        for (size_t i = 0; i < vec->capacity; i++) {

            int a_val = *( int* )a;
            int* rule_value = ( int* )vector_get(vec, i);

            if (rule_value == NULL) {
                return 0;
            }

            if (a_val == *rule_value) {
                return 1;
            }
        }
    }

    return 0;
};

int main() {
    const char* filename = "input.txt";
    int result_1 = 0, result_2 = 0;
    char str[5], line[256];

    FILE* fptr = fopen(filename, "r");
    vector* rules = vector_create(sizeof(pair));

    rules_ordering = hash_map_create();

    if (fptr == NULL) {
        printf("fopen() failed with file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line), fptr)) {
        pair p;
        if (sscanf(line, "%i|%i", &p.first, &p.second) != -1) {
            snprintf(str, 5, "%i", p.first);
            vector_push_back(rules, &p);

            vector* p_first_map = hash_map_find(rules_ordering, str);

            if (p_first_map == NULL || p_first_map->item_size < 0 ||
                p_first_map->item_size >= MAX_ALLOWED_SIZE ||
                p_first_map->count > p_first_map->capacity) {

                p_first_map = vector_create(sizeof(int), 10);
            }

            vector_push_back(p_first_map, &p.second);
            hash_map_insert(rules_ordering, str, p_first_map, sizeof(vector));
        } else {
            break;
        }
    };

    while (fgets(line, sizeof(line), fptr)) {
        vector* v = vector_create(sizeof(int));

        char* strDup = strdup(line);
        char* token = strtok(strDup, ",");

        while (token != NULL) {
            int value = atoi(token);
            vector_push_back(v, &value);
            token = strtok(NULL, ",");
        }

        int valid = 1;

        for (size_t i = 0; i < v->count; i++) {
            for (size_t j = i + 1; j < v->count; j++) {
                for (size_t k = 0; k < rules->count; k++) {

                    pair rule = *( pair* )vector_get(rules, k);
                    int a_value = *( int* )vector_get(v, i);
                    int b_value = *( int* )vector_get(v, j);

                    if (rule.second == a_value && rule.first == b_value) {
                        valid = 0;
                    }
                }
            }
        }

        if (valid) {
            result_1 += *( int* )vector_get(v, v->count / 2);
        } else {
            qsort(v->items, v->count, v->item_size, sort_function);
            result_2 += *( int* )vector_get(v, v->count / 2);
        }

        free(strDup);
        strDup = NULL;

        vector_free(v);
        free(v);
        v = NULL;
    };

    for (size_t i = 0; i < 100; i++) {
        int value = i;
        snprintf(str, 5, "%i", value);
        vector* p_first_map = hash_map_find(rules_ordering, str);

        if (p_first_map != NULL) {
            vector_free(p_first_map);
            p_first_map = NULL;
        }
    }

    vector_free(rules);
    free(rules);
    rules = NULL;

    hash_map_free(rules_ordering);
    free(rules_ordering);
    rules_ordering = NULL;

    printf("Task 01: result [%i]\n", result_1);
    printf("Task 02: result [%i]\n", result_2);
    return 0;
}