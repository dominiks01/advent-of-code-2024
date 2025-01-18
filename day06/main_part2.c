#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

char get_character_from_map(vector* vec, int i, int j) {
    vector* row = vector_get(vec, i);

    if (!row) {
        return 0;
    }

    return *( char* )vector_get(row, j);
}

void insert_character_into_map(vector* vec, int i, int j, char c) {
    vector* row = vector_get(vec, i);

    if (!row) {
        return;
    }

    vector_insert(row, &c, j);
}

void mark_visited(vector* vec, int i, int j, char c, int visited) {
    vector* row = vector_get(vec, i);

    if (!row) {
        return;
    }

    vector* col = vector_get(row, j);

    if (!col) {
        return;
    }

    vector_insert(col, &visited, direction_to_index(c));
}

int check_visited(vector* vec, int i, int j, char c) {
    vector* row = vector_get(vec, i);

    if (!row) {
        return -1;
    }

    vector* col = vector_get(row, j);

    if (!col) {
        return -1;
    }

    int index = direction_to_index(c);
    int value = *( int* )vector_get(col, index);

    if (value != 0 && value != 1) {
        return -1;
    }

    return (value == 1) ? 1 : 0;
}

void print_map(vector* v) {
    printf("------------TEST----------------\n");
    for (size_t i = 0; i < v->count; i++) {
        vector* row = ( vector* )vector_get(v, i);
        for (size_t j = 0; j < row->count; j++) {
            printf("%c ", *( char* )vector_get(row, j));
        }
        printf("\n");
    }
    printf("------------TEST----------------\n");
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
        vector* row = ( vector* )vector_get(directions_counted, i);

        if (!row) {
            return;
        }

        for (size_t j = 0; j < row->count; j++) {
            vector* col = ( vector* )vector_get(row, j);

            if (!col) {
                return;
            }

            reset_vector_to_zero(col);
        }
    }
}

int main() {
    const char* filename = "input.txt";
    char line[256];
    char new_obstacle_char = '#', empty_place = '.', starting_direction = {};
    int result_2 = 0;

    pair map_size = {0, 0};
    FILE* fptr = fopen(filename, "r");
    vector* map = vector_create(sizeof(vector));
    pair starting_location = {.first = 0, .second = 0};

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
                starting_location.first = i;
                starting_location.second = row_count;
                starting_direction = line[i];
            }

            map_size.first = (i > map_size.first) ? i : map_size.first;
        }

        vector_push_back(map, row);
        free(row);
        row_count++;
    };

    map_size.second = row_count;
    vector* directions_counted = vector_create(sizeof(vector));

    for (int i = 0; i < map_size.first; i++) {
        vector* row = vector_create(sizeof(vector));
        for (int j = 0; j < map_size.second; j++) {
            vector* col = vector_create(sizeof(int));

            for (int k = 0; k < 4; k++) {
                int _ = 0;
                vector_push_back(col, &_);
            }

            vector_push_back(row, col);
            free(col);
        }
        vector_push_back(directions_counted, row);
        free(row);
    }

    for (int i = 0; i < map_size.first; i++) {
        for (int j = 0; j < map_size.second; j++) {

            if ((i * map_size.first + j) % 100 == 0)
                printf("Calculating progress: [%i / %i]\n",
                       i * map_size.first + j,
                       map_size.first * map_size.second);

            if (get_character_from_map(map, i, j) != '.') {
                continue;
            }

            insert_character_into_map(map, i, j, new_obstacle_char);

            // Initailize starting parameters
            pair curr_pos = starting_location;
            char char_move = starting_direction;
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
                    map, next_pos.second, next_pos.first);

                while (object_at_next_pos == '#') {
                    char_move = rotate_move_direction(char_move);
                    move_val = move_direction(char_move);
                    next_pos = move_direction_sum(curr_pos, move_val);

                    object_at_next_pos = get_character_from_map(
                        map, next_pos.second, next_pos.first);
                }

                if (check_visited(directions_counted, curr_pos.second,
                                  curr_pos.first, char_move)) {
                    result_2++;
                    break;
                } else {
                    mark_visited(directions_counted, curr_pos.second,
                                 curr_pos.first, char_move, 1);
                }
            }

            insert_character_into_map(map, i, j, empty_place);
            reset_directions_counted(directions_counted);
        }
    }

    for (size_t i = 0; i < map->count; i++) {
        vector* row = vector_get(map, i);
        vector_free(row);
    }

    for (size_t i = 0; i < directions_counted->count; i++) {
        vector* row = vector_get(directions_counted, i);
        for (size_t j = 0; j < row->count; j++) {
            vector* col = vector_get(row, j);
            vector_free(col);
        }
        free(row->items);
    }
    free(directions_counted->items);
    free(directions_counted);
    free(map->items);
    free(map);

    printf("Task 02: result [%i]\n", result_2);
    return 0;
}