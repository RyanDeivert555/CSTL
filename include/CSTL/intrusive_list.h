#pragma once
#include "common.h" // IWYU pragma: keep

typedef struct IntrusiveNode {
    struct IntrusiveNode* next;
} IntrusiveNode;

void IntrusiveNodeInsertAfter(IntrusiveNode* node, IntrusiveNode* new_node);

typedef struct IntrusiveList {
    IntrusiveNode* head;
} IntrusiveList;

#define ParentOf(T, field, child) ((T*)((U8*)(child) - offsetof(T, field)))

IntrusiveNode* IntrusiveListPopFront(IntrusiveList* list);
void IntrusiveListPushFront(IntrusiveList* list, IntrusiveNode* new_node);
