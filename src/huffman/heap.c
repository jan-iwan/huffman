#include "heap.h"

#include <stdlib.h>

inline static uint16_t parent(uint16_t i) {
    return (i-1)/2;
}
inline static uint16_t left(uint16_t i) {
    return 2*i + 1;
}
inline static uint16_t right(uint16_t i) {
    return 2*i + 2;
}

// swap 2 nodes in heap
static void swap(MinHeap* heap, uint16_t index1, uint16_t index2) {
    Node tmp = heap->pq[index1];
    heap->pq[index1] = heap->pq[index2];
    heap->pq[index2] = tmp;
}

static void heapify(MinHeap* heap, uint16_t root) {
    uint16_t l = left(root);
    uint16_t r = right(root);

    uint16_t smallest = root;

    if(l < heap->size && heap->pq[l].freq < heap->pq[root].freq)
        smallest = l;

    if(r < heap->size && heap->pq[r].freq < heap->pq[smallest].freq)
        smallest = r;

    if(smallest != root) {
        swap(heap, root, smallest);
        heapify(heap, smallest);
    }
}

static void check_upwards(MinHeap* heap, uint16_t i) {
    while(i != 0 && heap->pq[parent(i)].freq > heap->pq[i].freq) {
        swap(heap, i, parent(i));
        i = parent(i);
    }
}

bool make_min_heap(MinHeap* heap, const freq_t* key) {
    heap->size = 0;

    // populate heap with data from dict
    for(int i = 0; i < 256; i++) {
        if(key[i] != 0) {
            Node new_node = (Node){i, key[i], NULL, NULL};
            push(heap, new_node);
        }
    }

    heapify(heap, 0);

    return 0;
}

bool push(MinHeap* heap, Node element) {
    if(heap->size >= 256) {
        return 1;
    }

    uint16_t i = heap->size++;

    heap->pq[i] = element;

    check_upwards(heap, i);

    return 0;
}

Node* pop(MinHeap* heap) {
    if(heap->size <= 0) {
        return NULL;
    }

    Node* root = malloc(sizeof(Node));

    if(root == NULL)
        return NULL;

    *root = heap->pq[0];

    if(heap->size == 1) {
        heap->size--;
        return root;
    }

    heap->pq[0] = heap->pq[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return root;
}

#ifdef VERBOSE

#include <stdio.h>

void print_node(Node* node) {
    printf( MAGENTA "  s: " YELLOW BOLD "%c " DIM "(%d)\n" RESET
            MAGENTA "  f: " GREEN "%u\n"
            MAGENTA "  l: " GREEN "%p\n"
            MAGENTA "  r: " GREEN "%p\n" RESET,
            node->symbol, node->symbol, node->freq, node->left, node->right);
}

#endif
