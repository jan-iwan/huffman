#include "arguments.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman/verbose.h"
#include "error.h"

#define MAX_ARG_COUNT 3

// passsing "--" as a parameter means that any string after it
// is not an option (like "-h" or "--help" )
static bool end_of_options = false;

// true if string starts with '-'.
// if string is simply "-" it means "read from stdin"
// so it's not an option to parse.
// if end_of_options is true return false
static bool is_option(char* str) {
    return str[0] == '-' && str[1] != '\0' && !end_of_options;
}

// long options are "--whatever"
// "--" is allowed
static bool is_long_option(char* str) {
    return str[1] == '-';
}

// each long option corresponds to a particular flag
// e.g. "--compress" is the same as "-c"
static char long_opt_to_flag(char* str) {
    if(!strcmp(str, "--compress"))
        return 'c';

    else if(!strcmp(str, "--decompress"))
        return 'd';

    else if(!strcmp(str, "--help"))
        return 'h';

    else if(!strcmp(str, "--version"))
        return 'V';

    else {
        fprintf(stderr, RED "Invalid option: " YELLOW BOLD "%s\n", str);
        error(BAD_USAGE, NULL);
    }
    return 0;
}

static char* program_name;

static void print_version() {
    puts("huffman v0.1");
}

static void print_help() {
    puts(GREEN "huffman " RESET "v0.1");

    puts("Compress and decompress FILE using huffman coding.\n");

    printf(YELLOW "Usage: " RESET "%s [OPTIONS] [FILE]\n\n", program_name);

    puts(
        YELLOW "Options:\n" RESET
        "  -c, --compress       Compress FILE. Default behavior.\n"
        "  -d, --decompress     Decompress FILE. Default if file\n"
        "                       is \".hfmn\"\n"
        // "  -v, --verbose        NOT IMPLEMENTED"
        "  -V, --version        Print program information.\n"
        "  -h, --help           Print this help.\n"
        // ...
        "\n"
        "If FILE is not given or it is \"-\", read from  stdin."
        );
}

// false if neither "-c" nor "-d" were specified
static bool compress_specified = false;

// "-c" and "-d" should be mutually exclusive
static void set_opt(Options* opts, char flag) {
    switch (flag) {
        case 'c':
            opts->compress = true;
            compress_specified = true;
            break;

        case 'd':
            opts->compress = false;
            compress_specified = true;
            break;

        case 'h':
            print_help();
            exit(0);

        // case 'v':
        //     opts->verbose++;

        case 'V':
            print_version();
            exit(0);

        default:
            fprintf(stderr, RED "Invalid option: " YELLOW BOLD "%c\n", flag);
            error(BAD_USAGE, NULL);
    }
}

static void parse_opts(Options* opts, char* str) {
    if(is_long_option(str)) {
        // if str is "--"
        if(str[2] == '\0') {
            end_of_options = true;
        }

        // parse long option ("--whatever")
        else {
            char flag = long_opt_to_flag(str);
            set_opt(opts, flag);
        }
    }

    // if str is something like "-cvv" (same as "-c" "-v" "-v")
    else {
        // loop through each character in str
        for(int i = 1; i < strlen(str); i++) {
            set_opt(opts, str[i]);
        }
    }
}

bool has_extension(const char* path, const char* extension) {
    const char* path_ext = strrchr(path, '.');

    // if there is no extension
    if(path_ext == NULL)
        return false;

    return !strcmp(path_ext + 1, extension);
}

char* parse_args(int argc, char** argv, Options* opts) {
    if(argc <= 1)
        error(BAD_USAGE, NULL);

    program_name = argv[0];

    char* path = NULL;

    // loop through each string in argv
    for(int i = 1; i < argc; i++) {
        if(is_option(argv[i])) {
            parse_opts(opts, argv[i]);
        }

        // if string is not an option
        // and path hasn't been set yet
        else if(path == NULL) {
            path = argv[i];
        }
    }

    if(!compress_specified) {
        // compression by default
        opts->compress = true;

        // if file extension is ".hfmn" and compression was not specified
        // decompress by default
        if(has_extension(path, "hfmn"))
            opts->compress = false;
    }

    // by default read from stdin
    if(path == NULL)
        path = "-";

    return path;
}

// #ifdef VERBOSE
//
// void print_args(Arg paths, Options *opts) {
//     TITLE("Arguments");
//     fprintf(stderr, YELLOW "Mode: " RESET YELLOW BOLD "%s\n" RESET, opts->compress ? "Compress" : "Decompress");
//
//     fprintf(stderr, CYAN "File: " WHITE BOLD "%s\n" RESET, paths.argv[0]);
//     if(paths.argc == 2) {
//         fprintf(stderr, CYAN "Key: " WHITE BOLD "%s\n" RESET, paths.argv[1]);
//     }
// }
//
// #endif
