#include "CSTL/intrusive_list.h"

void IntrusiveNodeInsertAfter(IntrusiveNode* node, IntrusiveNode* new_node) {
    new_node->next = node->next;
    node->next = new_node;
}

IntrusiveNode* IntrusiveListPopFront(IntrusiveList* list) {
    if (!list->head) {
        return NULL;
    }

    IntrusiveNode* first = list->head;
    list->head = first->next;

    return first;
}

void IntrusiveListPushFront(IntrusiveList* list, IntrusiveNode* new_node) {
    new_node->next = list->head;
    list->head = new_node;
}
