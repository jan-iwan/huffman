#pragma once

/*
    simple but hopefully useful and good looking
    error handling
*/

enum Error {
    DUMMY_ERROR,
    BAD_USAGE,
    // BAD_KEY,
    OPEN_FILE,
    READ_FILE,
    WRITE_FILE,
    CLOSE_FILE,
    INVALID_FILE,
};

// print error message and exit the program
void error(const enum Error E, const char* str);
