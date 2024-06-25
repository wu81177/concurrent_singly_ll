#include <stdio.h>
#include <limits.h>
#include "singly_ll.h"

void print_list(list_t* list) {
    if (list == NULL || list->head == NULL) {
        printf("List is empty or not initialized.\n");
        return;
    }

    node_t* curr = list->head;
    while (curr != NULL) {
        if (curr->data == INT_MIN) {
            printf("HEAD -> ");
        } else if (curr->data == INT_MAX) {
            printf("TAIL\n");
        } else {
            printf("%d -> ", curr->data);
        }

        curr = curr->next;
    }

    printf("NULL\n");
}

int main() {
    list_t *list = list_new();
    if (!list) {
        printf("Failed to create list\n");
        return 1;
    }

    add_tail(list, 10);
    add_tail(list, 20);
    add_tail(list, 30);
    add_tail(list, 60);
    add_tail(list, 54);

    printf("Initial list:\n");
    print_list(list);
    // node_t *left_node = NULL;
    // list_search(list, 20, &left_node);

    printf("Remove 20\n");
    list_remove(list,20);
    print_list(list);

    // node_t *left_node = NULL;
    // node_t *found_node = list_search(list, 20, &left_node);
    // if (found_node) {
    //     printf(" found_node: %p \n", (void *)found_node);
    //     if (found_node->data == 20) {
    //         printf("Found node with value 20\n");
    //     }
    // } else {
    //     printf("Node with value 20 not found\n");
    // }

    // found_node = list_search(list, 40, &left_node);
    // if (found_node) {
    //     printf(" found_node: %p \n", (void *)found_node);
    //     if (found_node->data == 40) {
    //         printf("Found node with value 40\n");
    //     }
    // } else {
    //     printf("Node with value 40 not found\n");
    // }

    return 0;
}
