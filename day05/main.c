#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../utils/graph.h"
#include "./../utils/hash_map.h"
#include "./../utils/vector.h"

int calculateValue(vector* updates, HashMap* rules) {
    char stringA[20];
    char stringB[20];

    for (size_t i = 0; i < updates->count; i++) {
        for (size_t j = i + 1; j < updates->count; j++) {

            int vala = *( int* )get(updates, i);
            int valb = *( int* )get(updates, j);

            snprintf(stringA, sizeof(stringA), "%i", vala);
            snprintf(stringB, sizeof(stringB), "%i", valb);

            int updateRuleA = *( int* )search_map(rules, stringA);
            int updateRuleB = *( int* )search_map(rules, stringB);

            if (updateRuleA == -1 || updateRuleB == -1) continue;
            if (updateRuleA >= updateRuleB) return 0;
        }
    }

    printf("VAL: [%i]\n", *( int* )get(updates, updates->count / 2));
    return *( int* )get(updates, updates->count / 2);
};

int main(int argc, char** argv) {

    char line[256];
    const char* filename = "input.txt";
    int first = 0, second = 0, value = 0, index = 0;
    char str[3];

    int result = 0;

    FILE* fptr = fopen(filename, "r");

    if (fptr == NULL) {
        printf("fopen() failed with file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    vector* v = create_vector(sizeof(vector*), 100);

    for (int i = 0; i < 100; i++) {
        vector* s = create_vector(sizeof(int), 10);

        for (int j = 0; j < 10; j++) {
            int value = i * j + 31;
            push_back(s, &value);
        }

        push_back(v, &s);
    }

    while (fgets(line, sizeof(line), fptr)) {
        if (sscanf(line, "%i|%i", &first, &second) != -1) {

            vector* act = ( vector* )get(v, first);
            push_back(act, &second);
            emplace_vector(v, act, first);

        } else {
            break;
        }
    };

    for (size_t i = 0; i < 100; i++) {
        vector* act = ( vector* )get(v, first);
        print_vector(act);
    }

    // while (fgets(line, sizeof(line), fptr)) {

    //     char* strDup = strdup(line);
    //     char* token = strtok(strDup, ",");

    //     while (token != NULL) {
    //         int value = atoi(token);
    //         token = strtok(NULL, ",");
    //     }
    // };

    printf("Task 01: result [%i]\n", result);

    return 0;
}