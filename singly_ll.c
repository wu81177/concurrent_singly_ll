#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "singly_ll.h"
#include "atomics.h"

static inline bool is_marked_ref(void *i)
{
    return (bool) ((uintptr_t) i & 0x1L);
}

static inline void *get_unmarked_ref(void *w)
{
    return (void *) ((uintptr_t) w & ~0x1L);
}

static inline void *get_marked_ref(void *w)
{
    return (void *) ((uintptr_t) w | 0x1L);
}

static node_t *new_node(val_t val, node_t *next)
{
    node_t *node = malloc(sizeof(node_t));
    node->data = val;
    node->next = next;
    return node;
}

list_t *list_new()
{
    list_t *the_list = malloc(sizeof(list_t));

    the_list->head = new_node(INT_MIN, NULL);
    the_list->tail = new_node(INT_MAX, NULL);
    the_list->head->next = the_list->tail;
    the_list->size = 0;
    return the_list;
}

bool add_tail(list_t *the_list, val_t val) {
    if (the_list == NULL) return false;

    node_t *new_elem = new_node(val, NULL);
    if (new_elem == NULL) return false;

    while (1) {
        node_t *tail = the_list->tail;
        node_t *prev = the_list->head;

        while (prev->next != tail) {
            prev = prev->next;
        }

        new_elem->next = tail;
        if (CAS_PTR(&(prev->next), tail, new_elem) == tail) {
            FAI_U32(&(the_list->size));
            return true;
        }
    }
}


node_t *list_search(list_t *set, val_t val, node_t **left_node) {
    node_t *left_node_next = NULL, *right_node = NULL;

    while (1) {
        node_t *t = set->head;
        node_t *t_next = t->next;

        while (t != set->tail) {

            if (!is_marked_ref(t_next)) {
                *left_node = t;
                left_node_next = t_next;
            }

            t = get_unmarked_ref(t_next);

            if (t->data == val || t == set->tail) {
                break;
            }
            t_next = t->next;
        }

        right_node = t;

        printf("left_node: %" PRIdPTR " (%p), right_node: %" PRIdPTR " (%p)\n",
           *left_node != NULL ? (*left_node)->data : 0, 
           (void *)(*left_node),
           right_node != NULL ? right_node->data : 0, 
           (void *)right_node);

        if (left_node_next == right_node) {
            if (!is_marked_ref(right_node->next)) {
                return right_node;
            }
        } else {
            if (CAS_PTR(&((*left_node)->next), left_node_next, right_node) == left_node_next) {
                if (!is_marked_ref(right_node->next)) {
                    return right_node;
                }
            }
        }
    }
}

bool list_remove(list_t *the_list, val_t val) {
    bool removed = false;

    while (1) {
        node_t *left = NULL;
        node_t *right = list_search(the_list, val, &left);

        /* check if we found our node */
        if ((right == the_list->tail) || (right->data != val)) {
            return removed;  // Return true if any element was removed
        }

        node_t *right_succ = right->next;
        if (!is_marked_ref(right_succ)) {
            if (CAS_PTR(&(right->next), right_succ, get_marked_ref(right_succ)) == right_succ) {
                FAD_U32(&(the_list->size));
                removed = true;
            }
        }
    }
}



