#pragma once
#include <stdlib.h>
#include "allocator.h" // IWYU pragma: keep

#define LIST_DEFINE(T) \
    typedef struct node_##T { \
        T data; \
        struct node_##T* next; \
    } node_##T; \
    \
    node_##T* node_##T##_new(allocator allocator, T entry); \
    \
    typedef struct list_##T { \
        node_##T* head; \
        size_t length; \
        allocator allocator; \
    } list_##T; \
    \
    list_##T list_##T##_new(allocator allocator); \
    void list_##T##_free(list_##T* list); \
    void list_##T##_push_front(list_##T* list, T entry); \
    void list_##T##_push_back(list_##T* list, T entry); \
    T list_##T##_pop_front(list_##T* list); \
    T list_##T##_pop_back(list_##T* list); \

#define LIST_IMPLEMENT(T) \
    node_##T* node_##T##_new(allocator allocator, T entry) { \
        node_##T* result = allocator_raw_alloc(allocator, sizeof(node_##T), 1); \
        result->data = entry; \
        result->next = NULL; \
        \
        return result; \
    } \
    \
    list_##T list_##T##_new(allocator allocator) { \
        list_##T result = {0}; \
        result.allocator = allocator; \
        \
        return result; \
    } \
    \
    void list_##T##_free(list_##T* list) { \
        node_##T* current = list->head; \
        while (current) { \
            node_##T* temp = current; \
            current = current->next; \
            allocator_raw_free(list->allocator, temp, sizeof(node_##T), 1); \
        } \
    } \
    \
    void list_##T##_push_front(list_##T* list, T entry) { \
        node_##T* new_node = node_##T##_new(list->allocator, entry); \
        list->length++; \
        if (list->head == NULL) { \
            list->head = new_node; \
            return; \
        } \
        node_##T* old_head = list->head; \
        list->head = new_node; \
        list->head->next = old_head; \
    } \
    \
    void list_##T##_push_back(list_##T* list, T entry) { \
        node_##T* new_node = node_##T##_new(list->allocator, entry); \
        list->length++; \
        if (list->head == NULL) { \
            list->head = new_node; \
            return; \
        } \
        node_##T* current = list->head; \
        while (current && current->next) { \
            current = current->next; \
        } \
        current->next = new_node; \
    } \
    \
    T list_##T##_pop_front(list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        node_##T* temp = list->head; \
        T value = temp->data; \
        list->head = list->head->next; \
        allocator_raw_free(list->allocator, temp, sizeof(node_##T), 1); \
        \
        return value; \
    } \
    \
    T list_##T##_pop_back(list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        if (list->head->next == NULL) { \
            T value = list->head->data; \
            allocator_raw_free(list->allocator, list->head, sizeof(node_##T), 1); \
            list->head = NULL; \
            \
            return value; \
        } \
        node_##T* second_last = list->head; \
        while (second_last->next->next) { \
            second_last = second_last->next; \
        } \
        T value = second_last->next->data; \
        allocator_raw_free(list->allocator, second_last->next, sizeof(node_##T), 1); \
        second_last->next = NULL; \
        \
        return value; \
    }
