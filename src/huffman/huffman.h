#pragma once

#include "common.h"
#include "key.h"

typedef struct {
    array file;
    freq_t* key;
} HuffmanPair;

// compress an array of bytes
// returns a pair containing the compressed array and its key
HuffmanPair compress(const array uncompressed);

// decompress an array of bytes using a key
// returns decompressed array
array decompress(const HuffmanPair compressed);
