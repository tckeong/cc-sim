#include <print>

#include "config.hpp"
#include "simulator.hpp"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::print("Usage: {} <protocol> <input_file> <cache_size (Default: 4 KiB)> <associativity "
                   "(Default: 2)> <block_size (Default: 32 bytes)>\n",
                   argv[0]);
        return 1;
    }

    string cc_protocol(argv[1]);
    string input_file(argv[2]);
    string cache_size(argc > 3 ? argv[3] : "");
    string associativity(argc > 4 ? argv[4] : "");
    string block_size(argc > 5 ? argv[5] : "");

    Config    config(cc_protocol, input_file, cache_size, associativity, block_size);
    Simulator simulator(config);

    simulator.run();

    return 0;
}
