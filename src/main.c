// WIP

/*  #\\~                        ~//#  *
 *          Huffman encoding          *
 *          implemented in C          *
 *            from scratch            *
 *  #\\~                        ~//#  */

#include "arguments.h"
#include "huffman/huffman.h"
#include "fileio.h"

int main(int argc, char* argv[]) {
    // parse arguments
    Options opts;
    char* path = parse_args(argc, argv, &opts);

    // compress file
    if(opts.compress) {
        array uncompressed = load_file(path);

        HuffmanPair compressed = compress(uncompressed);

        save_pair(compressed, path);

        // clean(uncompressed, compressed);
    }

    // decompress file
    else {
        HuffmanPair compressed = load_pair(path);

        array uncompressed = decompress(compressed);

        save_file(uncompressed, path);

        // clean(uncompressed, compressed);
    }

    return 0;
}
