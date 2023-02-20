#pragma once

/*
    #~ -                       -- TODO: --                      - ~#
    key could be way smaller:
    keys could use uint8_t since the actual freq doesn't matter.
    this could be done when generating the huffman tree (pq is used)
    each node's freq is the sum of the freq of its children.
    could also be done in gen_key() to be more structured
*/

#include "common.h"

// a key is an array containing the frequencies
// of each possible byte in some other array
// it's a table of frequencies.
// e.g. key['a'] = <freq of 'a' in text>

// an array of 256 elements whould be enough
// to store the frequency of all possible chars
typedef uint64_t freq_t;

#define KEY_SIZE BYTEMAX * sizeof(freq_t)

// read file and calculate frequency of each byte
freq_t* gen_key(const array file);

// calculate the size of the array that was used
// to generate the key.
// this will be wrong if using some other file's key
const size_t guess_file_size(const freq_t* key);


void print_key(const freq_t* key);
// void print_key_count(freq_t* key);

