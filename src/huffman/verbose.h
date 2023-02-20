#pragma once

// verbosity can be disabled at compile
// time for a smaller executable
#define VERBOSE         

#ifndef NO_COLOR
#define RESET   "\x1b[0m"

#define BOLD    "\x1b[1m"
#define DIM     "\x1b[2m"
#define ITALIC  "\x1b[3m"
#define UNDERLINE "\x1b[4m"

#define RED     "\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW  "\x1b[33m"
#define BLUE    "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN    "\x1b[36m"
#define WHITE   "\x1b[37m"
#define DEFAULT "\x1b[39m"

#else
#define RESET   ""

#define BOLD    ""
#define DIM     ""
#define ITALIC  ""
#define UNDERLINE ""

#define RED     ""
#define GREEN   ""
#define YELLOW  ""
#define BLUE    ""
#define MAGENTA ""
#define CYAN    ""
#define WHITE   ""
#define DEFAULT ""
#endif

// print info to stderr
typedef enum {
    SILENT,
    PROGRESS,
    DEBUG,
} VerboseLevel;

#define TITLE(x) fputs(BOLD RED UNDERLINE x RESET "\n", stderr);

#ifdef VERBOSE

/*enum Color {
    bold,
    dim,
    italic,
    underline,

    red,
    green,
    yellow,
    blue,
    magenta,
    cyan,
    white,
    default_fg,
};*/

#endif
