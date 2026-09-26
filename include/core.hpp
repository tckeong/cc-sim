#ifndef CORE_HPP
#define CORE_HPP

#include "types.h"
#include "cache.hpp"
#include "stats.hpp"

enum class CoreOperation { LOAD, STORE, OTHER };

class Core {
public:
    Core() = default;
    Core(u32 core_idx, std::string input_file, u32 cache_size, u32 associativity, u32 block_size)
        : core_idx(core_idx), current_cycle(0), block_size(block_size), input_file(input_file),
          cache(Cache(cache_size, associativity, block_size)) {};

    void run(Stats &stats);

private:
    u32         core_idx;
    u32         current_cycle;
    u32         block_size;
    std::string input_file;
    Cache       cache;

    std::pair<u32, u32> parse_line(std::string line);
    CoreOperation       parse_core_operation(u32 label);
    void                increase_cycle(u32 cycle);
};

#endif // CORE_HPP
