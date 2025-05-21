#pragma once
#include "allocator.h" // IWYU pragma: keep
#include "common.h" // IWYU pragma: keep

#define LIST_DEFINE(T) \
    typedef struct Node_##T { \
        T data; \
        struct Node_##T* next; \
    } Node_##T; \
    \
    Node_##T* Node_##T##_New(Allocator allocator, T entry); \
    \
    typedef struct List_##T { \
        Node_##T* head; \
        i64 length; \
    } List_##T; \
    \
    List_##T List_##T##_New(void); \
    void List_##T##_Free(List_##T* list, Allocator allocator); \
    void List_##T##_PushFront(List_##T* list, Allocator allocator, T entry); \
    void List_##T##_PushBack(List_##T* list, Allocator allocator, T entry); \
    T List_##T##_PopFront(List_##T* list, Allocator allocator); \
    T List_##T##_PopBack(List_##T* list, Allocator allocator); \

#define LIST_IMPL(T) \
    Node_##T* Node_##T##_New(Allocator allocator, T entry) { \
        Node_##T* result = AllocatorAlloc(Node_##T, allocator, 1); \
        result->data = entry; \
        result->next = NULL; \
        \
        return result; \
    } \
    \
    List_##T List_##T##_New(void) { \
        List_##T result = {0}; \
        \
        return result; \
    } \
    \
    void List_##T##_Free(List_##T* list, Allocator allocator) { \
        Node_##T* current = list->head; \
        while (current) { \
            Node_##T* temp = current; \
            current = current->next; \
            AllocatorFree(Node_##T, allocator, temp, 1); \
        } \
    } \
    \
    void List_##T##_PushFront(List_##T* list, Allocator allocator, T entry) { \
        Node_##T* new_node = Node_##T##_New(allocator, entry); \
        list->length++; \
        if (list->head == NULL) { \
            list->head = new_node; \
            return; \
        } \
        Node_##T* old_head = list->head; \
        list->head = new_node; \
        list->head->next = old_head; \
    } \
    \
    void List_##T##_PushBack(List_##T* list, Allocator allocator, T entry) { \
        Node_##T* new_node = Node_##T##_New(allocator, entry); \
        list->length++; \
        if (list->head == NULL) { \
            list->head = new_node; \
            return; \
        } \
        Node_##T* current = list->head; \
        while (current && current->next) { \
            current = current->next; \
        } \
        current->next = new_node; \
    } \
    \
    T List_##T##_PopFront(List_##T* list, Allocator allocator) { \
        Assert(list->length > 0); \
        list->length--; \
        Node_##T* temp = list->head; \
        const T value = temp->data; \
        list->head = list->head->next; \
        AllocatorFree(Node_##T, allocator, temp, 1); \
        \
        return value; \
    } \
    \
    T List_##T##_PopBack(List_##T* list, Allocator allocator) { \
        Assert(list->length > 0); \
        list->length--; \
        if (list->head->next == NULL) { \
            const T value = list->head->data; \
            AllocatorFree(Node_##T, allocator, list->head, 1); \
            list->head = NULL; \
            \
            return value; \
        } \
        Node_##T* second_last = list->head; \
        while (second_last->next->next) { \
            second_last = second_last->next; \
        } \
        const T value = second_last->next->data; \
        AllocatorFree(Node_##T, allocator, second_last->next, 1); \
        second_last->next = NULL; \
        \
        return value; \
    }

