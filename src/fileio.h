#pragma once

/*
   small utility to read and write files
*/

#include "huffman/huffman.h"

// load a file into memory
array load_file(const char* path);

// load a pair into memory
HuffmanPair load_pair(const char* path);

// save an array to the filesystem
void save_file(array file, const char* path);

// save a pair to the filesystem
void save_pair(HuffmanPair pair, const char* path/* , bool merge */);

// #ifdef VERBOSE
//
// void print_file(array file);
//
// #endif
