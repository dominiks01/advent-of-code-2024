#ifndef GRAPH_MATRIX_REPRESENTATION_H
#define GRAPH_MATRIX_REPRESENTATION_H

#include <stdio.h>

typedef struct {
    int* matrix;
    size_t matrix_size;
    int directed;
} GraphMatrix;

GraphMatrix* graph_matrix_initialize(size_t V);

void graph_matrix_insert_edge(GraphMatrix*, size_t, size_t, int);

void graph_matrix_delete_edge(GraphMatrix*, size_t, size_t);

void graph_matrix_free(GraphMatrix*);

int* graph_matrix_topological_sort(GraphMatrix* g);

void graph_matrix_print_graph(GraphMatrix* g);

#endif