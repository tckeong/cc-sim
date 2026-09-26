#include "cache_line.hpp"

bool CacheLine::cache_read(u32 address, u32 current_cycle) {
    u32 tag = parse_tag(address);

    if (check_cache_hit(tag, current_cycle)) {
        return true;
    }

    resolve_cache_miss(tag, current_cycle);
    return false;
}

bool CacheLine::cache_write(u32 address, u32 current_cycle) {
    u32 tag = parse_tag(address);

    if (check_cache_hit(tag, current_cycle)) {
        return true;
    }

    resolve_cache_miss(tag, current_cycle);
    return false;
}

u32 CacheLine::parse_tag(u32 address) {
    u32 tag = address >> (cache_offset + block_offset);
    return tag;
}

bool CacheLine::check_cache_hit(u32 tag, u32 current_cycle) {
    for (u32 i = 0; i < associativity; i++) {
        if (valid_bits[i] && tags[i] == tag) {
            last_visit[i] = current_cycle;
            return true;
        }
    }

    return false;
}

void CacheLine::resolve_cache_miss(u32 tag, u32 current_cycle) {
    u32 lru_index = 0;
    u32 lru_cycle = last_visit[0];

    for (u32 i = 1; i < associativity; i++) {
        if (last_visit[i] < lru_cycle) {
            lru_index = i;
            lru_cycle = last_visit[i];
        }
    }

    valid_bits[lru_index] = true;
    tags[lru_index]       = tag;
    last_visit[lru_index] = current_cycle;
}