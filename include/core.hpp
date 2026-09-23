#ifndef CORE_HPP
#define CORE_HPP

#include "types.h"
#include "cache_line.hpp"

class Core {
public:
    Core() = default;
    Core(u32 core_idx, std::string input_file, u32 cache_size, u32 associativity, u32 block_size)
        : core_idx(core_idx), input_file(input_file),
          cache_count((cache_size / block_size) / associativity),
          cache_line(cache_count, CacheLine(associativity, block_size)) {};

    void run();

private:
    u32                    core_idx;
    std::string            input_file;
    u32                    cache_count;
    std::vector<CacheLine> cache_line;

    std::pair<u32, u32> parse_line(std::string line);
};

#endif // CORE_HPP
