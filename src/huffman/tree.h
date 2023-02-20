#pragma once

#include "heap.h"
#include "key.h"

// generate a binary tree using huffman algorithm
Node* gen_huffman_tree(const freq_t* key);

// recursively free nodes in tree
void delete_tree(Node* root);
