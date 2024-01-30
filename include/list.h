#pragma once
#include <stdlib.h>

#define LIST_DEFINE(T) \
    typedef struct node_##T { \
        T data; \
        struct node_##T* next; \
    } node_##T; \
    \
    node_##T* node_##T##_new(T entry) { \
        node_##T* result = malloc(sizeof(node_##T)); \
        result->data = entry; \
        result->next = NULL; \
        \
        return result; \
    } \
    \
    typedef struct list_##T { \
        node_##T* head; \
        size_t length; \
    } list_##T; \
    \
    list_##T list_##T##_new() { \
        list_##T result = {0}; \
        result.head = NULL; \
        result.length = 0; \
        \
        return result; \
    } \
    \
    void list_##T##_free(list_##T* list) { \
        node_##T* current = list->head; \
        while (current) { \
            node_##T* temp = current; \
            current = current->next; \
            free(temp); \
        } \
    } \
    \
    void list_##T##_push_front(list_##T* list, T entry) { \
        node_##T* new_node = node_##T##_new(entry); \
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
        node_##T* new_node = node_##T##_new(entry); \
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
    void list_##T##_pop_front(list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        node_##T* temp = list->head; \
        list->head = list->head->next; \
        free(temp); \
    } \
    \
    void list_##T##_pop_back(list_##T* list) { \
        assert(list->length > 0); \
        list->length--; \
        if (list->head->next == NULL) { \
            free(list->head); \
            list->head = NULL; \
            return; \
        } \
        node_##T* second_last = list->head; \
        while (second_last->next->next) { \
            second_last = second_last->next; \
        } \
        free(second_last->next); \
        second_last->next = NULL; \
    }
