#ifndef CACHE_HPP
#define CACHE_HPP

#include "types.h"
#include "cache_line.hpp"

#include <cmath>

class Cache {
public:
    Cache() = default;

    Cache(u32 cache_size, u32 associativity, u32 block_size)
        : cache_count((cache_size / block_size) / associativity) {
        block_offset = std::log2(block_size);
        cache_offset = std::log2(cache_count);

        cache_line = std::vector(cache_count, CacheLine(cache_offset, associativity, block_size));
    };

    bool access(CacheOperation operation, u32 address, u32 current_cycle);

private:
    u32                    block_offset;
    u32                    cache_offset;
    u32                    cache_count;
    std::vector<CacheLine> cache_line;

    u32 parse_cache_line_idx(u32 address);
};

#endif // CACHE_HPP
