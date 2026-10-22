#include <iostream>
#include <format>

#include "config.hpp"
#include "simulator.hpp"

int main(int argc, char *argv[])
{
    if (argc < 3) {
        std::cout << std::format(
            "Usage: {} <protocol> <input_file> <cache_size (Default: 4096 bytes)> <associativity "
            "(Default: 2)> <block_size (Default: 32 bytes)>\n",
            argv[0]);
        return 1;
    }

    std::string cc_protocol(argv[1]);
    std::string input_file(argv[2]);
    std::string cache_size(argc > 3 ? argv[3] : "");
    std::string associativity(argc > 4 ? argv[4] : "");
    std::string block_size(argc > 5 ? argv[5] : "");

    Config config(cc_protocol, input_file, cache_size, associativity, block_size);

    auto error_message = config.check();
    if (error_message) {
        std::cout << *error_message << std::endl;
        return 1;
    }

    Simulator simulator(config, 4);

    simulator.run();

    return 0;
}
