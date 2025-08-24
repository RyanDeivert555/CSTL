#include "intrusive_list.h"
#include "common.h"

void IntrusiveNodeInsertAfter(IntrusiveNode* node, IntrusiveNode* new_node) {
    Assert(new_node);
    new_node->next = node->next;
    node->next = new_node;
}

IntrusiveList IntrusiveListNew(void) {
    const IntrusiveList result = {0};

    return result;
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
    Assert(new_node);
    new_node->next = list->head;
    list->head = new_node;
}

