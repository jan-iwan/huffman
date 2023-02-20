#include "huffman.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "key.h"
#include "heap.h"
#include "tree.h"
#include "code.h"
#include "verbose.h"

#define BENCHMARK
#ifdef BENCHMARK

#include <time.h>

typedef struct {
    clock_t time;
    const char* name;
} Stamp;

// Stamp stamps[5];

typedef struct {
    uint stamp_count;
    Stamp* stamps;
} Bench;

Stamp _stamps[8];
Bench b = { 0, _stamps };

// inline void start_benchmark() {
//     b.stamps[b.stamp_count].time = clock();
// }

void add_time_stamp(const char* name) {
    b.stamps[b.stamp_count++] = (Stamp){ .time = clock(), .name = name };
}

// inline void end_benchmark() {
//     b.stamps[b.stamp_count].time = clock();
// }

void print_benchmark() {
    TITLE("Benchmark");
    clock_t total_time = b.stamps[b.stamp_count-1].time - b.stamps[0].time;

    fprintf(stderr, GREEN BOLD "Total: " YELLOW BOLD "%fms\n" RESET,
            (double)total_time / 1000);

    for(int i = 1; i < b.stamp_count; i++) {
        clock_t duration = b.stamps[i].time - b.stamps[i-1].time;

        fprintf(stderr, GREEN "%s: " BLUE BOLD "%.1f%% " RESET YELLOW DIM "(%fms)\n" RESET,
                b.stamps[i].name,
                100.0 * (double)duration / total_time,
                (double)duration / 1000);
    }
}

#define time_stamp(x) add_time_stamp(x)
#else
#define time_stamp(x) ;
#endif

typedef struct {
    float scalar; char* unit;
} Size;
static char* units[] = { "B", "KiB", "MiB", "GiB" };
Size readable_size(size_t bytes) {
    Size size = { .scalar = bytes, .unit = units[0] };

    for(int i = 1; i < 4; i++) {
        bytes /= 0x400;
        if(bytes > 0) {
            size.unit = units[i];
            size.scalar /= (float)0x400;
        }
    }

    return size;
}

HuffmanPair compress(const array uncompressed) {
    // add_time_stamp("Start");
    time_stamp("start");

    // if key is not given, create one
    // if(key == NULL)

    // generate table with frequency of symbols in text
    freq_t* key = gen_key(uncompressed);
    time_stamp("Key");

    // huffman algorithm
    Node* root = gen_huffman_tree(key);
    time_stamp("Tree");

    // calculate new codes for each symbol
    Code* dictionary = gen_dict(root);
    time_stamp("Dict");

    array encoded = encode(uncompressed, dictionary);
    time_stamp("Encode");

    // fprintf(stderr, "original.size = %lu\n", uncompressed.size);
    // fprintf(stderr, "encoded.size = %lu\n", encoded.size);

    // pack array and key in a struct
    HuffmanPair compressed = { encoded, key };

    print_benchmark();

    Size original_size = readable_size(uncompressed.size);
    fprintf(stderr, CYAN "\nOriginal: " GREEN "%.1f%s\n",
            original_size.scalar, original_size.unit);

    Size compressed_size = readable_size(compressed.file.size);
    fprintf(stderr, MAGENTA "Compressed: " GREEN BOLD "%.1f%s\n",
            compressed_size.scalar, compressed_size.unit);

    fprintf(stderr, YELLOW "Compressed by " RED "%.2f%%\n" RESET,
            100.0 * compressed.file.size / uncompressed.size);

    return compressed;
}

array decompress(const HuffmanPair compressed) {
    time_stamp("start");

    Node* root = gen_huffman_tree(compressed.key);
    time_stamp("Tree");

    size_t size = guess_file_size(compressed.key);
    time_stamp("guess");

    array uncompressed = decode(compressed.file, root, size);
    time_stamp("decode");

    print_benchmark();

    return uncompressed;
}
