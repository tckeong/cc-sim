#include "cache.hpp"

CacheAccessResult Cache::access(CacheOperation operation, u32 address, u32 current_cycle) {
    u32 cache_line_idx = parse_cache_line_idx(address);

    switch (operation) {
        case CacheOperation::READ:
            return cache_line[cache_line_idx].cache_read(address, current_cycle);
        case CacheOperation::WRITE:
            return cache_line[cache_line_idx].cache_write(address, current_cycle);
        default:
            break;
    }

    return CacheAccessResult{.cache_hit = false, .bus_update = false};
}

u32 Cache::parse_cache_line_idx(u32 address) {
    u32 mask           = (1 << cache_offset) - 1;
    u32 cache_line_idx = (address >> block_offset) & mask;

    return cache_line_idx;
}
