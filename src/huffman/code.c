#include "code.h"

#include <stdlib.h>

#ifdef VERBOSE

#include <stdio.h>

static void print_code(const Code code) {
    for(int i = code.lenght-1; i >= 0; i--)
        fprintf(stderr, "%c", code.value & 1 << i ? '1' : '0');
}

static void print_code_fancy(const Code code, const Node* const node) {
    // only print printable characters (avoid \n and stuff)
    byte symbol = node->symbol >= 0x20 && node->symbol <= 0x7e ? node->symbol : ' ' ;

    fprintf(stderr,  YELLOW BOLD "%c " DIM "(%d) " RESET MAGENTA "{%lu}: " RESET
            , symbol, node->symbol, node->freq);

    fputs(GREEN, stderr);
    print_code(code);
    fprintf(stderr, " [%d]\n" RESET, code.lenght);
}

static void print_colored(const Code code) {
    static char color = 0;

    if(color++%2) fprintf(stderr, MAGENTA);
    else fprintf(stderr, YELLOW);

    print_code(code);

    fprintf(stderr, RESET);
}

void print_binary(const array encoded) {
    for(uint32_t i = 0; i < encoded.size; i++) {
        if(i % 2) fprintf(stderr, BLUE);
        else fprintf(stderr, CYAN);

        fprintf(stderr, BINARY_PATTERN, BINARY(encoded.data[i]));
    }
    fprintf(stderr, "\n");
}

#endif

// 1011 becomes 01011
static Code append_0(Code code) {
    code.lenght++;
    return code;
}

// 1011 becomes 11011
static Code append_1(Code code) {
    code.value |= 1 << code.lenght++;
    return code;
}

// recursively traverse tree and calculate codes
static void calculate_codes(const Node* root, Code* dictionary, Code code) {
    // if node is lo the right, append a 0
    if(root->right != 0)
        calculate_codes(root->right, dictionary, append_0(code));

    // if node is lo the left, append a 1
    if(root->left != 0)
        calculate_codes(root->left, dictionary, append_1(code));

    // if node is leaf both child will be NULL
    if(root->left == root->right) {
        dictionary[root->symbol] = code;
        /* verbose */
        // print_code_fancy(code, root);
    }

    // // 1 being left and 0 being right is an arbitrary decision
    // else {
    //     // if node is lo the right, append a 0
    //     if(root->right != 0)
    //         calculate_codes(root->right, dictionary, append_0(code));
    //
    //     // if node is lo the left, append a 1
    //     if(root->left != 0)
    //         calculate_codes(root->left, dictionary, append_1(code));
    // }
}

Code* gen_dict(Node* root) {
    /* verbose */
    // TITLE("Get codes");

    Code* dictionary = malloc(BYTEMAX * sizeof(Code));

    // actual generation
    calculate_codes(root, dictionary, (Code){ 0, 0 });

    // tree won't be needed later
    delete_tree(root);

    return dictionary;
}

// where the bitwise magic happens...
// APPARENTLY WORKS FINE
array encode(const array uncompressed, Code* dictionary) {
    array encoded;

    // encoded array will always be <= uncompressed
    // so the same amount of memory will always be enough.
    // an extra element will be needed later
    // the array has to be empty for the or operator (|) to work
    encoded.data = calloc(uncompressed.size + 1, sizeof(byte));

    // encoded index
    // starts at 1 since element 0 will be used later
    size_t n = 1;

    // position inside a byte at which to start writing a code.
    // e.g. byte = 00000000, code = 111, offset = 2. Then: byte = 00011100
    uint8_t offset = 0;

    /* verbose */
    // TITLE("Encode");
    // fprintf(stderr, "Original: ");
    // print_binary(uncompressed);
    // fprintf(stderr, "Encoded: ");

    // loop through each uncompressed element
    for(size_t i = 0; i < uncompressed.size; i++) {
        // code corresponding to the current uncompressed byte
        Code code = dictionary[uncompressed.data[i]];

        // wider wiew into the encoded array
        // if a code exceeds 8 bits, part of it will be written to
        // one byte and the other to the following byte
        // this depends on the cpu being little endian (LSB first)
        // because offset 0 inside a uint32_t = offset  0 inside a uint8_t
        uint32_t* window = (void*)&encoded.data[n];

        // "paste" the code
        *window |= code.value << offset;

        // update offset
        offset += code.lenght;

        // update encoded index
        // move on by 0, 1 or 2 bytes
        // only has effect if offset >= 8
        n += offset / 8;

        // update offset
        // only has effect if offset >= 8
        offset %= 8;
    }

    // store final offset at the beginning of the array
    // this is the leftover of the last byte
    // this is why n started at 1 and why an extra byte was allocated
    encoded.data[0] = offset;

    encoded.size = n+1;

    // dictionary won't be needed later
    free(dictionary);

    // it is almost certain that encoded.size < file.size
    // which makes the original allocation wasteful.
    // this ensures that only the memory that is needed is used
    encoded.data = realloc(encoded.data, encoded.size);

    return encoded;
}

// where the magic happens again...
array decode(const array encoded, Node* root, const size_t size_guess) {
    array decoded;

    // size_guess will be accurate if the key that was used to compress the
    // file was the one generated with it.
    // if used some other key, size_guess will always be >= decoded.size
    decoded.size = size_guess;
    decoded.data = malloc(size_guess);

    size_t i = 0; // decoded index

    Node* node = root; // current node

    // n is the index in the encoded array
    // first element is skipped because it stores leftover of last byte
    // last element is treated specially
    for(size_t n = 1; n < encoded.size - 1; n++) {
        // bit is the index of the current bit in the current byte.
        // indexes in a byte: 7654 3210 (7 is msb)
        for(int8_t bit = 0; bit < 8; bit++) {
            bool bit_is_one = encoded.data[n] & 1 << bit;

            node = bit_is_one ? node->left : node->right;

            // if node is leaf both childs will be NULL
            if(node->left == node->right) {
                // found symbol
                // move on to the next decoded byte
                decoded.data[i++] = node->symbol;

                // reset search
                node = root;
            }
        }
    }

    // last byte may or may not be complete
    uint8_t leftover = encoded.data[0];

    for(char bit = 0; bit < leftover; bit++) {
        // check if a current bit is set
        bool bit_is_one = encoded.data[encoded.size-1] & 1 << bit;

        // 1 is left, 0 is right
        node = bit_is_one ? node->left : node->right;

        if(node->left == node->right) {
            decoded.data[i++] = node->symbol;

            // reset
            node = root;
        }
    }

    // tree won't be needed later
    delete_tree(root);

    // array could be smalled if size_guess was overestimated
    // this ensures that only the memory that is needed is used
    // decoded.data = realloc(decoded.data, decoded.size);

    return decoded;
}
