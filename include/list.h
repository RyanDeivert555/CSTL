#pragma once
#include <assert.h> // IWYU pragma: keep
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
    } list_##T; \
    \
    list_##T list_##T##_new(void); \
    void list_##T##_free(allocator allocator, list_##T* list); \
    void list_##T##_push_front(allocator allocator, list_##T* list, T entry); \
    void list_##T##_push_back(allocator allocator, list_##T* list, T entry); \
    T list_##T##_pop_front(allocator allocator, list_##T* list); \
    T list_##T##_pop_back(allocator allocator, list_##T* list); \

#define LIST_IMPL(T) \
    node_##T* node_##T##_new(allocator allocator, T entry) { \
        node_##T* result = allocator_create(node_##T, allocator); \
        result->data = entry; \
        result->next = NULL; \
        \
        return result; \
    } \
    \
    list_##T list_##T##_new(void) { \
        list_##T result = {0}; \
        \
        return result; \
    } \
    \
    void list_##T##_free(allocator allocator, list_##T* list) { \
        node_##T* current = list->head; \
        while (current) { \
            node_##T* temp = current; \
            current = current->next; \
            allocator_destroy(node_##T, allocator, temp); \
        } \
    } \
    \
    void list_##T##_push_front(allocator allocator, list_##T* list, T entry) { \
        node_##T* new_node = node_##T##_new(allocator, entry); \
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
    void list_##T##_push_back(allocator allocator, list_##T* list, T entry) { \
        node_##T* new_node = node_##T##_new(allocator, entry); \
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
    T list_##T##_pop_front(allocator allocator, list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        node_##T* temp = list->head; \
        T value = temp->data; \
        list->head = list->head->next; \
        allocator_destroy(node_##T, allocator, temp); \
        \
        return value; \
    } \
    \
    T list_##T##_pop_back(allocator allocator, list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        if (list->head->next == NULL) { \
            T value = list->head->data; \
            allocator_destroy(node_##T, allocator, list->head); \
            list->head = NULL; \
            \
            return value; \
        } \
        node_##T* second_last = list->head; \
        while (second_last->next->next) { \
            second_last = second_last->next; \
        } \
        T value = second_last->next->data; \
        allocator_destroy(node_##T, allocator, second_last->next); \
        second_last->next = NULL; \
        \
        return value; \
    }

