#include "graph_matrix_representation.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

GraphMatrix* graph_matrix_initialize(size_t V) {
    GraphMatrix* graph = ( GraphMatrix* )malloc(sizeof(GraphMatrix));
    if (!graph) {
        return NULL;
    }

    graph->matrix = calloc(V * V, sizeof(int));
    if (!graph->matrix) {
        return NULL;
    }

    graph->matrix_size = V;
    return graph;
}

void graph_matrix_insert_edge(GraphMatrix* g, size_t i, size_t j, int value) {
    if (j <= 0 || j > g->matrix_size) {
        printf("ERROR: out-of-bounds in function: %s\n", __FUNCTION__);
        return;
    }

    if (i <= 0 || i > g->matrix_size) {
        printf("ERROR: out-of-bounds in function: %s\n", __FUNCTION__);
        return;
    }

    g->matrix[i * g->matrix_size + j] = value;
    return;
}

void graph_matrix_delete_edge(GraphMatrix* g, size_t i, size_t j) {
    graph_matrix_insert_edge(g, i, j, 0);
}

static void topologicalSortUtil(GraphMatrix* g, int curr_vertex, int* visited,
                                int* stack, int* stack_index) {
    visited[curr_vertex] = 1;

    for (size_t i = 0; i < g->matrix_size; i++) {
        if (g->matrix[curr_vertex * g->matrix_size + i] && visited[i] == 0) {
            topologicalSortUtil(g, i, visited, stack, stack_index);
        }
    }

    stack[(*stack_index)++] = curr_vertex;
}

int* graph_matrix_topological_sort(GraphMatrix* g) {
    int visited[g->matrix_size] = {};
    int* stack = calloc(g->matrix_size, sizeof(int));
    int stack_index = 0;

    for (size_t i = 0; i < g->matrix_size; i++) {
        if (!visited[i])
            topologicalSortUtil(g, i, visited, stack, &stack_index);
    }

    return stack;
}

int graph_matrix_search_edge(GraphMatrix* g, size_t i, size_t j) {
    if (j <= 0 || j > g->matrix_size) {
        printf("ERROR: out-of-bounds in function: %s\n", __FUNCTION__);
        return 0;
    }

    if (i <= 0 || i > g->matrix_size) {
        printf("ERROR: out-of-bounds in function: %s\n", __FUNCTION__);
        return 0;
    }

    return g->matrix[i * g->matrix_size + j];
}

void graph_matrix_free(GraphMatrix* g) {
    free(g->matrix);
    free(g);
}

void graph_matrix_print_graph(GraphMatrix* g) {
    for (size_t i = 0; i < g->matrix_size * g->matrix_size; i++) {
        printf(" %i ", g->matrix[i]);

        if (i % g->matrix_size == 0) {
            printf("\n");
        }
    }
}