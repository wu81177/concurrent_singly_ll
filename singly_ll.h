#ifndef SINGLY_LL_H
#define SINGLY_LL_H

#include <stdbool.h>
#include <stdint.h>

typedef intptr_t val_t;

typedef struct node {
    val_t data;
    struct node *next;
} node_t;

typedef struct {
    node_t *head;
    node_t *tail;
    uint32_t size;
} list_t;

list_t *list_new();
bool add_tail(list_t *the_list, val_t val);
node_t *list_search(list_t *set, val_t val, node_t **left_node);
void print_list(list_t* list);

#endif
