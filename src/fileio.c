/*
    load_pair() can crash if the file is not a valid pair
*/

#include "fileio.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "arguments.h"
#include "huffman/common.h"
#include "huffman/key.h"
#include "huffman/verbose.h"

// first, tiny layer of abstraction to avoid boilerplate code
// and handle errors

typedef struct {
    const char* path;
    FILE* ptr;
    size_t size;
} File;

enum permission {
    READ,
    WRITE,
};

static size_t get_file_size(File file) {
    // move to 0 bytes before the end of the file
    if(fseek(file.ptr, 0L, SEEK_END))
        return 0;

    // get current location
    size_t size = ftell(file.ptr);

    // go back to the beginning of the file
    rewind(file.ptr);

    return size;
}

static File open_file(const char* path, enum permission perm) {
    File file;

    char* mode = perm == READ ? "rb" : "wb";

    file.path = path;
    file.ptr = fopen(file.path, mode);

    // error handling
    if(!file.ptr)
        error(OPEN_FILE, file.path);

    file.size = get_file_size(file);

    return file;
}

static array read_file(File file) {
    array contents;

    contents.size = file.size;

    // error handling
    if(contents.size == 0)
        error(OPEN_FILE, file.path);

    contents.data = malloc(contents.size);
    size_t read = fread(contents.data, sizeof(byte), contents.size, file.ptr);

    // error handling
    if(read != contents.size) {
        free(contents.data);
        error(READ_FILE, file.path);
    }

    return contents;
}

static void write_file(File file, array contents) {
    size_t written = fwrite(contents.data, sizeof(byte), contents.size, file.ptr);

    if(written != contents.size)
        error(WRITE_FILE, file.path);
}

static void close_file(File file) {
    if(fclose(file.ptr))
        error(CLOSE_FILE, file.path);
}


array load_file(const char* path) {
    File f = open_file(path, READ);

    array contents = read_file(f);

    close_file(f);

    return contents;
}

HuffmanPair load_pair(const char* path) {
    File file = open_file(path, READ);

    // check_pair_integrity(file);
    // or maybe
    // check_pair_integrity(contents);

    array contents = read_file(file);

    // temporary until check_pair_integrity() is implemented
    if(contents.size < KEY_SIZE)
        error(INVALID_FILE, file.path);

    // key is stored at the beginning of the file
    freq_t* key = (void*)contents.data;

    // compressed array is stored just afer key
    array compressed;
    compressed.size = file.size - KEY_SIZE;

    // scary pointer arithmetic right here
    compressed.data = contents.data + KEY_SIZE;

    close_file(file);

    HuffmanPair pair = { compressed, key };
    return pair;
}

static char* remove_extension(const char* path, const char* ext) {
    char* str = malloc(strlen(path));

    strcpy(str, path);

    if(has_extension(path, ext)) {
        // "file.hfmn" -> "file\0hfmn"
        str[strlen(path) - (strlen(ext) + 1)] = '\0';
    }

    return str;
}

void save_file(array uncompressed, const char* path) {
    char* file_path = remove_extension(path, "hfmn");

    File file = open_file(file_path, WRITE);

    write_file(file, uncompressed);

    close_file(file);

    free(file_path);
}

static char* append_extension(const char* path, const char* ext) {
    char* str = malloc(strlen(path) + strlen(ext) + 1);

    strcpy(str, path);
    strcat(str, ".");
    strcat(str, ext);

    return str;
}

void save_pair(HuffmanPair pair, const char* path) {
    char* pair_path = append_extension(path, "hfmn");

    File file = open_file(pair_path, WRITE);

    array key = { .data = (void*)pair.key, .size = KEY_SIZE };
    write_file(file, key);

    array compressed = pair.file;
    write_file(file, compressed);

    close_file(file);

    free(pair_path);
}

#ifdef VERBOSE

void print_text(array file) {
    fprintf(stderr, GREEN "text" YELLOW " (%lu): " RESET, file.size);

    // just in case
    fwrite(file.data, sizeof(uint8_t), file.size, stderr);
    fputs("\n", stderr);
}

#endif
