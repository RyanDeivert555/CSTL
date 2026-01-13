#pragma once
#include "common.h" // IWYU pragma: keep

typedef struct intrusive_node {
    struct intrusive_node* next;
} intrusive_node;

void intrusive_node_insert_after(intrusive_node* node, intrusive_node* new_node);

typedef struct intrusive_list {
    intrusive_node* head;
} intrusive_list;

#define parent_of(T, field, child) ((T*)((u8*)(child) - offsetof(T, field)))

intrusive_node* intrusive_list_pop_front(intrusive_list* list);
void intrusive_list_push_front(intrusive_list* list, intrusive_node* new_node);
