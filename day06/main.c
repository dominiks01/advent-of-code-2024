#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../utils/pair.h"
#include "./../utils/vector.h"

char rotate_move_direction(char actual_direction) {
    switch (actual_direction) {
        case '>':
            return 'v';
        case 'v':
            return '<';
        case '<':
            return '^';
        case '^':
            return '>';
        default:
            printf("error: unknown character: %s\n", &actual_direction);
            return 0;
    }
}

pair move_direction(char direction) {
    pair move = {0, 0};
    switch (direction) {
        case '<':
            move.first = -1;
            break;
        case '>':
            move.first = 1;
            break;
        case 'v':
            move.second = 1;
            break;
        case '^':
            move.second = -1;
            break;
        default:
            break;
    }
    return move;
}

pair move_direction_sum(pair a, pair b) {
    pair result = {a.first + b.first, a.second + b.second};
    return result;
}

int main() {
    const char* filename = "test.txt";
    char line[256];
    char place_visited = 'X';
    char direction = {};
    int result_1 = 0, result_2 = 0;

    pair new_position = {};
    pair map_size;

    FILE* fptr = fopen(filename, "r");
    vector* map = vector_create(sizeof(vector));

    pair current_location = {.first = 0, .second = 0};

    if (fptr == NULL) {
        printf("fopen() failed with file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int row_count = 0;

    while (fgets(line, sizeof(line), fptr)) {
        vector* row = vector_create(sizeof(char));

        for (int i = 0; line[i] != '\0'; i++) {
            vector_push_back(row, &line[i]);

            if (line[i] == '<' || line[i] == '>' || line[i] == '^' ||
                line[i] == 'v') {
                current_location.first = i;
                current_location.second = row_count;
                direction = line[i];
            }

            map_size.first = (i > map_size.first) ? i : map_size.first;
        }

        vector_push_back(map, row);

        vector_free(row);
        // free(row);
        row_count++;
    };

    map_size.second = row_count;

    for (size_t i = 0; i < map->count; i++) {
        vector* row = ( vector* )vector_get(map, i);
        for (size_t j = 0; j < row->count; j++) {
            printf("%c", *( char* )vector_get(row, j));
        }

        printf("\n");
    }

    pair move_direction_value = {0, 0}, next_position = {0, 0};

    while (1) {

        vector* row_value = ( vector* )vector_get(map, current_location.second);
        char object_at_pos =
            *( char* )vector_get(row_value, new_position.first);

        if (object_at_pos == '.') {
            result_1++;
            vector_insert(row_value, &place_visited, new_position.first);
        }

        move_direction_value = move_direction(direction);
        new_position =
            move_direction_sum(current_location, move_direction_value);
        next_position = move_direction_sum(new_position, move_direction_value);

        if (next_position.first >= map_size.first || next_position.first < 0) {
            break;
        }

        if (next_position.second >= map_size.second ||
            next_position.second < 0) {
            break;
        }

        vector* row = ( vector* )vector_get(map, next_position.second);
        char object = *( char* )vector_get(row, next_position.first);

        if (object == '#') {
            direction = rotate_move_direction(direction);
        }

        current_location = new_position;
    }

    for (size_t i = 0; i < map->count; i++) {
        // vector* row = vector_get(map, i);
        // vector_free(row);
        // free(row);
    }

    vector_free(map);
    free(map);

    printf("Task 01: result [%i]\n", result_1 + 1);
    printf("Task 02: result [%i]\n", result_2);
    return 0;
}