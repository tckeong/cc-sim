#include "cache_line.hpp"

CacheAccessResult CacheLine::cache_read(u32 address, u32 current_cycle) {
    u32 tag       = parse_tag(address);
    i32 hit_index = check_cache_hit(tag, current_cycle);

    if (hit_index >= 0) {
        return CacheAccessResult{.cache_hit = true, .bus_update = false};
    }

    auto [lru_index, bus_update] = resolve_cache_miss(tag, current_cycle);
    return CacheAccessResult{.cache_hit = false, .bus_update = bus_update};
}

CacheAccessResult CacheLine::cache_write(u32 address, u32 current_cycle) {
    u32 tag       = parse_tag(address);
    i32 hit_index = check_cache_hit(tag, current_cycle);

    if (hit_index >= 0) {
        dirty_bits[hit_index] = true;
        return CacheAccessResult{.cache_hit = true, .bus_update = false};
    }

    auto [lru_index, bus_update] = resolve_cache_miss(tag, current_cycle);
    dirty_bits[lru_index]        = true;
    return CacheAccessResult{.cache_hit = false, .bus_update = bus_update};
}

u32 CacheLine::parse_tag(u32 address) {
    u32 tag = address >> (cache_offset + block_offset);
    return tag;
}

i32 CacheLine::check_cache_hit(u32 tag, u32 current_cycle) {
    for (u32 i = 0; i < associativity; i++) {
        if (valid_bits[i] && tags[i] == tag) {
            last_visit[i] = current_cycle;
            return i;
        }
    }

    return -1;
}

std::pair<u32, bool> CacheLine::resolve_cache_miss(u32 tag, u32 current_cycle) {
    u32 lru_index = 0;
    u32 lru_cycle = last_visit[0];

    for (u32 i = 1; i < associativity; i++) {
        if (last_visit[i] < lru_cycle) {
            lru_index = i;
            lru_cycle = last_visit[i];
        }
    }

    bool bus_update = dirty_bits[lru_index];

    valid_bits[lru_index] = true;
    dirty_bits[lru_index] = false;
    tags[lru_index]       = tag;
    last_visit[lru_index] = current_cycle;

    return {lru_index, bus_update};
}