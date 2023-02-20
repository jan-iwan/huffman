#pragma once

#include <stdbool.h>
#include "common.h"
#include "key.h"

typedef struct node {
    byte symbol;
    freq_t freq;
    struct node* left;
    struct node* right;
} Node;

typedef struct {
    // pq should be stack allocated
    // nodes in the array may or may not
    Node pq[256];
    uint16_t size;
} MinHeap;


// MinHeap constructor
bool make_min_heap(MinHeap* heap, const freq_t* key);

// insert new element
bool push(MinHeap* heap, Node element);

// extract min (i.e. root)
Node* pop(MinHeap* heap);

#ifdef VERBOSE

void print_node(Node* node);

#endif
