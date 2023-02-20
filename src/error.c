#include "error.h"

#include <stdlib.h>
#include <stdio.h>
#include "huffman/verbose.h"

#define ERROR       RED BOLD "Error: " RESET
#define ARGUMENT    WHITE
#define ENDLINE     RESET "\n"

static const char* const error_messages[] = {
    [DUMMY_ERROR]   = "Dummy error",
    [BAD_USAGE]     = "Bad usage. Try `huffman -h` or `huffman --help` for more information",
    // [BAD_KEY]       = "Invalid key file",
    [OPEN_FILE]     = "Could not open file",
    [READ_FILE]     = "Could not open file for reading",
    [WRITE_FILE]    = "Could not open file for writing",
    [CLOSE_FILE]    = "Could not close file",
    [INVALID_FILE]  = "File is not valid for decompression",
};

void error(const enum Error error, const char* str) {
    if(str == NULL)
        fprintf(stderr, ERROR "%s." ENDLINE,
                error_messages[error]);

    else
        fprintf(stderr, ERROR "%s: " ARGUMENT "%s" ENDLINE,
                error_messages[error], str);

    exit(error);
}
