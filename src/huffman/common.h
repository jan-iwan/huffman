#pragma once

/*
   common definitions
   avoid those annoying #include issues where it's
   needed to define a type and then include a header
*/

#include <stdint.h>
#include <stddef.h>

typedef uint8_t byte;

// number of possible bytes (2**8)
#define BYTEMAX 256

// array of bytes
typedef struct {
    byte* data;
    size_t size;
} array;

