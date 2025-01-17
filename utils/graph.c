#include "graph.h"

#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

AdjListNode* create_adj_list_node(int dest) {

    AdjListNode* new_node = ( AdjListNode* )malloc(sizeof(AdjListNode));
    new_node->dest = dest;
    new_node->next = NULL;

    return new_node;
}

Graph* create_graph(int V) {
    Graph* new_graph = ( Graph* )malloc(sizeof(Graph));
    new_graph->V = V;
    new_graph->array = ( AdjList* )malloc(sizeof(AdjList) * V);

    for (int i = 0; i < new_graph->V; i++) new_graph->array[i].head = NULL;

    return new_graph;
}

void add_edge(Graph* graph, int src, int dest) {
    AdjListNode* new_node = create_adj_list_node(dest);
    new_node->next = graph->array[src].head;
    new_node->dest = dest;
    graph->array[src].head = new_node;

    return;
}

void print_graph(Graph* graph) {
    for (int i = 0; i < graph->V; i++) {

        AdjListNode* crawl = graph->array[i].head;
        printf("Vertex %d:", i);

        while (crawl) {
            printf(" -> %d", crawl->dest);
            crawl = crawl->next;
        }
        printf("\n");
    }
}

void delete_edge(Graph* graph, int src, int dest) {
    AdjListNode* temp = graph->array[src].head;
    AdjListNode* prev = NULL;

    while (temp != NULL && temp->dest != dest) {
        prev = temp;
        temp = temp->next;
    }

    if (temp != NULL) {
        if (prev != NULL)
            prev->next = temp->next;
        else
            graph->array[src].head = temp->next;
        free(temp);
    }

    return;
}

int search_edge(Graph* graph, int src, int dest) {
    AdjListNode* tmp = graph->array[src].head;

    while (tmp != NULL) {
        if (tmp->dest == dest) return 1;
        tmp = tmp->next;
    }

    return 0;
}

void topological_sort_helper(Graph* g, int v, bool* visited,
                             AdjListNode** list) {
    visited[v] = true;

    AdjListNode* current = g->array[v].head;

    while (current != NULL) {
        int value = current->dest;
        if (!visited[value]) topological_sort_helper(g, value, visited, list);
        current = current->next;
    }

    AdjListNode* newNode = create_adj_list_node(v);
    newNode->next = *list;
    *list = newNode;

    return;
}

AdjListNode* topological_sort(Graph* g) {
    AdjListNode* result = NULL;
    bool* visited = ( bool* )malloc(g->V * sizeof(bool));

    for (int i = 0; i < g->V; ++i) visited[i] = false;
    for (int i = 0; i < g->V; ++i)
        if (!visited[i]) topological_sort_helper(g, i, visited, &result);

    free(visited);
    return result;
}

void free_adj_list_node(AdjListNode* current) {

    while (current != NULL) {
        AdjListNode* temp = current;
        current = current->next;
        free(temp);
    }
}

void free_graph(Graph* h) {
    if (h == NULL) return;

    for (int i = 0; i < h->V; i++) {
        AdjListNode* current = h->array[i].head;
        while (current != NULL) {
            AdjListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(h->array);
    free(h);
}
