#pragma once
#include "vec.h"

// TODO: can i use my linked list? (maybe not due to redefinitions)
// FIXME: should hash limit to capacity or length?
#define HASHMAP_DEFINE(K, V) \
    typedef struct node_##K##_to_##V { \
        K key; \
        V value; \
        struct node_##K##_to_##V* next; \
    } node_##K##_to_##V; \
    \
    node_##K##_to_##V* node_##K##_to_##V##_new(K key, V value) { \
        node_##K##_to_##V* result = malloc(sizeof(node_##K##_to_##V)); \
        result->key = key; \
        result->value = value; \
        result->next = NULL; \
        \
        return result; \
    } \
    \
    typedef node_##K##_to_##V* node_##K##_to_##V##_ptr; \
    VEC_DEFINE(node_##K##_to_##V##_ptr) \
    typedef struct hashmap_##K##_to_##V { \
        vec_node_##K##_to_##V##_ptr nodes; \
        size_t (*hasher)(K); \
    } hashmap_##K##_to_##V; \
    \
    hashmap_##K##_to_##V hashmap_##K##_to_##V##_new(size_t (*hasher)(K)) { \
        hashmap_##K##_to_##V result = {0}; \
        result.nodes = vec_node_##K##_to_##V##_ptr_new(); \
        result.hasher = hasher; \
        \
        return result; \
    } \
    \
    /* hasher which ensures the hash is within capacity */ \
    size_t hashmap_##K##_to_##V##_hash(hashmap_##K##_to_##V hashmap, K key) { \
        return hashmap.hasher(key) % hashmap.nodes.capacity; \
    } \
    \
    void hashmap_##K##_to_##V##_insert(hashmap_##K##_to_##V* hashmap, K key, V value) { \
        \
    }
