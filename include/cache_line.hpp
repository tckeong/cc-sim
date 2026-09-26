#ifndef CACHE_LINE_HPP
#define CACHE_LINE_HPP

#include "types.h"

#include <cmath>

class CacheLine {
public:
    CacheLine() = default;
    CacheLine(u32 cache_offset, u32 associativity, u32 block_size)
        : cache_offset(cache_offset), associativity(associativity), block_size(block_size),
          valid_bits(associativity, false), tags(associativity, 0), last_visit(associativity, 0) {
        block_offset = std::log2(block_size);
    };

    bool cache_read(u32 address, u32 current_cycle);
    bool cache_write(u32 address, u32 current_cycle);

private:
    u32 cache_offset;
    u32 associativity;
    u32 block_size;
    u32 block_offset;

    std::vector<bool> valid_bits;
    std::vector<u32>  tags;
    std::vector<u32>  last_visit;

    u32  parse_tag(u32 address);
    bool check_cache_hit(u32 tag, u32 current_cycle);
    void resolve_cache_miss(u32 tag, u32 current_cycle);
};

#endif // CACHE_LINE_HPP
