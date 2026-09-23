#ifndef CACHE_LINE_HPP
#define CACHE_LINE_HPP

#include "types.h"
#include "params.h"

class CacheLine {
public:
    CacheLine()
        : associativity(DEFAULT_ASSOCIATIVITY), block_size(DEFAULT_BLOCK_SIZE),
          cache(associativity, std::vector<u8>(block_size, 0)) {};

    CacheLine(u32 associativity, u32 block_size)
        : associativity(associativity), block_size(block_size),
          cache(associativity, std::vector<u8>(block_size, 0)) {};

private:
    u32                          associativity;
    u32                          block_size;
    std::vector<std::vector<u8>> cache;
};

#endif // CACHE_LINE_HPP
