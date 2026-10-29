#ifndef CACHE_HPP
#define CACHE_HPP

#include "types.hpp"
#include "cache_line.hpp"

#include <cmath>

class Cache
{
public:
    Cache() = default;

    Cache(u32 cache_size, u32 associativity, u32 block_size, u32 core_idx)
        : cache_count((cache_size / block_size) / associativity),
          core_idx(core_idx),
          cache_line()
    {
        block_offset = std::log2(block_size);
        cache_offset = std::log2(cache_count);
        cache_line.reserve(cache_count);

        for (u32 i = 0; i < cache_count; i++) {
            cache_line.emplace_back(cache_offset, associativity, block_size, core_idx);
        }
    };

    CacheAccessResult access(u32 address, u64 current_cycle);

    bool fill_cache_line(u32 address, u64 current_cycle, MesiState new_mesi_state);

    MesiState get_cache_line_mesi_state(u32 address);
    void      update_cache_line_mesi_state(u32 address, MesiState new_mesi_state);

private:
    u32                    block_offset;
    u32                    cache_offset;
    u32                    cache_count;
    u32                    core_idx;
    std::vector<CacheLine> cache_line;

    u32 parse_cache_line_idx(u32 address);
};

#endif // CACHE_HPP
