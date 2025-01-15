#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

typedef struct AdjListNode {
    int dest;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList {
    AdjListNode* head;
} AdjList;

typedef struct Graph {
    int V;
    AdjList* array;
} Graph;

AdjListNode* new_adj_list_node(int);
Graph* create_graph(int);
void add_edge(Graph*, int, int);
void delete_edge(Graph*, int, int);
int search_edge(Graph*, int, int);

AdjListNode* topological_sort(Graph*);
void topological_sort_helper(Graph*, int, bool*, AdjListNode**);
void free_graph(Graph*);
void free_adj_list_node(AdjListNode*);
void print_graph(Graph*);

#endif