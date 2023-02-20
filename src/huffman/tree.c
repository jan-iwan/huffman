/*
    TODO: tree should be stored as an array
    inexisting nodes could have freq = 0
    This whould reduce the call to malloc() and free()
    and probably make the decoder faster
*/

#include "tree.h"

#include <stdlib.h>
#include <stdio.h>
#include "verbose.h"

#ifdef VERBOSE

static void print_node(Node* node) {
    if(node->left == node->right) {
        byte symbol = node->symbol >= 0x20 && node->symbol <= 0x7e ? node->symbol : ' ' ;
        // found leaf
        fprintf(stderr, GREEN DIM "    Symbol: " RESET GREEN "%c (%d)\n", symbol, node->symbol);
        fprintf(stderr, GREEN DIM "    Freq: " RESET GREEN "%lu\n", node->freq);
    }

    else {
        fprintf(stderr, YELLOW DIM "    Freq: " RESET YELLOW "%lu\n", node->freq);
        fprintf(stderr, YELLOW DIM "    Left: " RESET YELLOW "%p\n", node->left);
        fprintf(stderr, YELLOW DIM "    Right: " RESET YELLOW "%p\n", node->right);
    }
}

static void print_new_node(Node node) {
    fprintf(stderr, RED BOLD "NEW NODE\n" RESET);

    fprintf(stderr, BLUE BOLD "    left child @ %p:\n" RESET, node.left);
    print_node(node.left);

    fprintf(stderr, BLUE BOLD "    right child @ %p:\n" RESET, node.right);
    print_node(node.right);

    puts(RESET);
}

#endif

#define NO_SYMBOL 0

// builds linked list based tree
Node* gen_huffman_tree(const freq_t* key) {
    // build priority queue from dict.
    // priority is compared by the
    // frequency of each symbol
    MinHeap heap;
    make_min_heap(&heap, key);

    // if there is only 1 possible symbol
    if(heap.size == 1) {
        Node* right = pop(&heap);

        Node root;
        root.symbol = NO_SYMBOL;
        root.freq = right->freq;
        root.right = right;
        root.left = NULL;

        push(&heap, root);
    }

    // build the tree from bottom to top
    while(heap.size > 1) {
        // pop 2 least frequent nodes.
        // putting the least frequent element at
        // the left is an arbitrary decision
        Node* left = pop(&heap);
        Node* right = pop(&heap);

        // create new node in huffman tree
        Node new_node;
        new_node.symbol = NO_SYMBOL;
        new_node.freq = left->freq + right->freq;
        new_node.left = left;
        new_node.right = right;

        // print_new_node(new_node);

        // push the new node into the queue
        push(&heap, new_node);
    }

    Node* root = pop(&heap);

    return root;

}

// recursively free nodes in tree
void delete_tree(Node* root) {
    if(root != 0) {
        delete_tree(root->left);
        delete_tree(root->right);

        free(root);
    }
}
