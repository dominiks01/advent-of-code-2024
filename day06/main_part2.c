#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "./../utils/pair.h"
#include "./../utils/vector.h"

int direction_to_index(char actual_direction) {
    switch (actual_direction) {
        case '>':
            return 0;
        case 'v':
            return 1;
        case '<':
            return 2;
        case '^':
            return 3;
        default:
            printf("error: unknown character: %s\n", &actual_direction);
            return -1;
    }
}

char get_character_from_map(vector* vec, int i, int j, pair map_size) {
    return *( char* )vector_get(vec, i * map_size.first + j);
}

void insert_char_into_map(vector* vec, int i, int j, char c, pair map_size) {
    vector_insert(vec, &c, i * map_size.first + j);
}

void mark_visited(vector* vec, int i, int j, char c, int v, pair map_size) {
    vector* values = vector_get(vec, i * map_size.first + j);
    if (!values) {
        return;
    }

    vector_insert(values, &v, direction_to_index(c));
}

int check_visited(vector* vec, int i, int j, char c, pair map_size) {
    vector* values = vector_get(vec, i * map_size.first + j);

    if (!values) {
        return -1;
    }

    int index = direction_to_index(c);
    int value = *( int* )vector_get(values, index);

    if (value != 0 && value != 1) {
        return -1;
    }

    return (value == 1) ? 1 : 0;
}

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

void reset_vector_to_zero(vector* v) {
    for (size_t i = 0; i < v->count; i++) {
        int zero = 0;
        vector_insert(v, &zero, i);
    }
}

void reset_directions_counted(vector* directions_counted) {
    for (size_t i = 0; i < directions_counted->count; i++) {
        vector* values = ( vector* )vector_get(directions_counted, i);

        if (!values) {
            return;
        }

        reset_vector_to_zero(values);
    }
}

void print_map(vector* vec, pair map_dimentions, pair guard_pos) {

    printf("---------------TEST--------------\n");
    for (int i = 0; i < map_dimentions.first; i++) {
        for (int j = 0; j < map_dimentions.second; j++) {
            if (i == guard_pos.first && j == guard_pos.second) {
                printf("X");
            } else {
                char act_val =
                    *( char* )vector_get(vec, i * map_dimentions.first + j);
                printf("%c ", act_val);
            }
        }
        printf("\n");
    }
    printf("---------------TEST--------------\n");
}

int validate_map(vector* map, vector* d_counted, pair s_pos, char char_m,
                 pair map_size) {

    pair curr_pos = s_pos;
    char char_move = char_m;
    pair move_val = {0, 0};
    pair next_pos = {};

    while (1) {
        move_val = move_direction(char_move);
        curr_pos = move_direction_sum(curr_pos, move_val);
        next_pos = move_direction_sum(curr_pos, move_val);

        if (next_pos.first < 0 || next_pos.first >= map_size.first ||
            next_pos.second < 0 || next_pos.second >= map_size.second) {
            break;
        }

        char object_at_next_pos = get_character_from_map(
            map, next_pos.second, next_pos.first, map_size);

        while (object_at_next_pos == '#') {
            char_move = rotate_move_direction(char_move);
            move_val = move_direction(char_move);
            next_pos = move_direction_sum(curr_pos, move_val);

            object_at_next_pos = get_character_from_map(
                map, next_pos.second, next_pos.first, map_size);
        }

        if (check_visited(d_counted, curr_pos.second, curr_pos.first, char_move,
                          map_size)) {
            return 1;
        } else {
            mark_visited(d_counted, curr_pos.second, curr_pos.first, char_move,
                         1, map_size);
        }
    }

    return 0;
}

int main() {

    clock_t t;
    t = clock();

    const char* filename = "input.txt";
    char line[256];
    char new_obstacle_char = '#', empty_place = '.', starting_direction = {};
    int result_2 = 0;

    pair map_dimentions = {0, 0};
    FILE* fptr = fopen(filename, "r");
    vector* map = vector_create(sizeof(char), 0);
    pair starting_location = {.first = 0, .second = 0};

    if (fptr == NULL) {
        printf("fopen() failed with file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int row_count = 0;

    while (fgets(line, sizeof(line), fptr)) {
        for (int i = 0; line[i] != '\0'; i++) {

            if (line[i] == '\n') {
                continue;
            }

            vector_push_back(map, &line[i]);

            if (line[i] == '<' || line[i] == '>' || line[i] == '^' ||
                line[i] == 'v') {
                starting_location.first = i;
                starting_location.second = row_count;
                starting_direction = line[i];
            }

            map_dimentions.first =
                (i > map_dimentions.first) ? i : map_dimentions.first;
        }

        row_count++;
    };

    map_dimentions.first = row_count;
    map_dimentions.second = row_count;

    vector* directions_counted = vector_create(sizeof(vector), map->count);

    for (int i = 0; i < map_dimentions.first; i++) {
        for (int j = 0; j < map_dimentions.second; j++) {
            vector* values = vector_create(sizeof(int), 4);

            for (int k = 0; k < 4; k++) {
                int _ = 0;
                vector_push_back(values, &_);
            }

            vector_push_back(directions_counted, values);
            free(values);
        }
    }

    for (int i = 0; i < map_dimentions.first; i++) {
        for (int j = 0; j < map_dimentions.second; j++) {

            printf("Work in progress [%i / %i]\n", i * map_dimentions.first + j,
                   map_dimentions.first * map_dimentions.second);

            if (get_character_from_map(map, i, j, map_dimentions) != '.') {
                continue;
            }

            insert_char_into_map(map, i, j, new_obstacle_char, map_dimentions);
            result_2 += validate_map(map, directions_counted, starting_location,
                                     starting_direction, map_dimentions);
            insert_char_into_map(map, i, j, empty_place, map_dimentions);
            reset_directions_counted(directions_counted);
        }
    }

    vector_free(map);

    for (size_t i = 0; i < directions_counted->count; i++) {
        vector* col = vector_get(directions_counted, i);
        vector_free(col);
        free(col->items);
    }

    free(directions_counted->items);
    free(directions_counted);
    free(map->items);
    free(map);

    t = clock() - t;
    double time_taken = (( double )t) / CLOCKS_PER_SEC;
    printf("Task 02: result [%i], time: [%.2f s]\n", result_2, time_taken);

    return 0;
}