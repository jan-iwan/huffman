#include "key.h"

#include <stdlib.h>
#include "common.h"
#include "error.h"

// read file and calculate frequency of each byte
freq_t* gen_key(const array file) {
    // set memory to zero with calloc()
    freq_t* key = calloc(BYTEMAX, sizeof(freq_t));

    for(unsigned int i = 0; i < file.size; i++)
        key[file.data[i]] += 1;

    return key;
}

// guess the size of the original array
// by summing up all frequencies
const size_t guess_file_size(const freq_t* key) {
    size_t count = 0;

    for(int i = 0; i < BYTEMAX; i++) {
        count += key[i];
    }
    return count;
}



#include <stdio.h>


void print_key(const freq_t* key) {
    FILE* fptr = fopen("key", "w");

    for(int i = 0; i < BYTEMAX; i++) {
        if(key[i])
            fprintf(fptr, "%d, ", i);
    }
    fprintf(fptr, "\n");
    for(int i = 0; i < BYTEMAX; i++) {
        if(key[i])
            fprintf(fptr, "%lu, ", key[i]);
    }

    fclose(fptr);
}

// void print_key(const freq_t* key) {
//     if(g_verbose >= DEBUG)
//     for(int i = 0; i < BYTEMAX; i++) {
//         if(key[i] != 0)
//             fprintf(stderr,
//                     GREEN "%c " CYAN "(%d)" WHITE ": " MAGENTA "%d" RESET "\n",
//                     i, i, key[i]);
//     }
// }

/*void print_key_count(freq_t* key) {
    uint32_t count = 0;

    for(int i = 0; i < BYTEMAX; i++)
        if(key[i] != 0) count++;

    fprintf(stderr, "dict count = %d\n", count);
}*/
