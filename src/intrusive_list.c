#include "CSTL/intrusive_list.h"

void intrusive_node_insert_after(intrusive_node* node, intrusive_node* new_node) {
    new_node->next = node->next;
    node->next = new_node;
}

intrusive_node* intrusive_list_pop_front(intrusive_list* list) {
    if (!list->head) {
        return NULL;
    }

    intrusive_node* const first = list->head;
    list->head = first->next;

    return first;
}

void intrusive_list_push_front(intrusive_list* list, intrusive_node* new_node) {
    new_node->next = list->head;
    list->head = new_node;
}
