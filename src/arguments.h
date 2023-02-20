#pragma once

/*
    argument parsing
*/

#include <stdbool.h>

// well...
typedef struct {
    bool compress;          // true if compressing, false if decompressing
    // bool merge;          // false if writing to a separate key file
    // VerboseLevel verbose;// could be SILENT, INFO or DEBUG
    // bool stdout_output;  // output file is stdout
    // bool stdin_input;    // input file is stdin
    // bool override        // allow overriding file
} Options;

char* parse_args(int argc, char** argv, Options* opts);
bool has_extension(const char* path, const char* extension);

// #ifdef VERBOSE
//
// void print_args(Arg paths, Options* opts);
//
// #endif
