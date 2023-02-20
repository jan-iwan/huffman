#pragma once

#include "tree.h"
#include "verbose.h"

// struct for huffman codes, which are variable in lenght
// therefore it is necessary to keep track of that
typedef struct {
    uint16_t value;
    uint8_t lenght; // in bits
} Code;

// void get_codes(const Node* root, Code* dict, Code code);
Code* gen_dict(Node* root);

// convert bytes
array encode(const array uncompressed, Code* dict);

array decode(const array encoded, Node* root, const size_t size_guess);

#ifdef VERBOSE

#define BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0')

void print_binary(const array encoded);

#endif
