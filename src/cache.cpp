#include "cache.hpp"

CacheAccessResult Cache::access(u32 address, u64 current_cycle)
{
    u32 cache_line_idx = parse_cache_line_idx(address);
    return cache_line[cache_line_idx].lookup(address, current_cycle);
}

bool Cache::fill_cache_line(u32 address, u64 current_cycle, MesiState new_mesi_state)
{
    u32 cache_line_idx = parse_cache_line_idx(address);
    return cache_line[cache_line_idx].fill(address, current_cycle, new_mesi_state);
}

MesiState Cache::get_cache_line_mesi_state(u32 address)
{
    u32 cache_line_idx = parse_cache_line_idx(address);
    return cache_line[cache_line_idx].get_mesi_state(address);
}

void Cache::update_cache_line_mesi_state(u32 address, MesiState new_mesi_state)
{
    u32 cache_line_idx = parse_cache_line_idx(address);
    cache_line[cache_line_idx].update_mesi_state(address, new_mesi_state);
}

u32 Cache::parse_cache_line_idx(u32 address)
{
    u32 mask           = (1 << cache_offset) - 1;
    u32 cache_line_idx = (address >> block_offset) & mask;

    return cache_line_idx;
}
