#include "cache_line.hpp"
#include "types.hpp"

CacheAccessResult CacheLine::lookup(u32 address, u64 current_cycle)
{
    u32 tag       = parse_tag(address);
    i32 hit_index = get_hit_index(tag);

    if (hit_index < 0) {
        return CacheAccessResult(false, MesiState::INVALID);
    }

    last_visit[hit_index] = current_cycle;
    return CacheAccessResult(true, mesi_states[hit_index].get_state());
}

bool CacheLine::fill(u32 address, u64 current_cycle, MesiState new_mesi_state)
{
    u32 lru_index = 0;

    for (u32 i = 0; i < associativity; i++) {
        if (!mesi_states[i].is_valid()) {
            lru_index = i;
            break;
        }

        if (last_visit[i] < last_visit[lru_index]) {
            lru_index = i;
        }
    }

    bool need_writeback = mesi_states[lru_index].get_state() == MesiState::MODIFIED;

    tags[lru_index]       = parse_tag(address);
    last_visit[lru_index] = current_cycle;
    mesi_states[lru_index].set_state(new_mesi_state);

    return need_writeback;
}

MesiState CacheLine::get_mesi_state(u32 address)
{
    u32 tag       = parse_tag(address);
    i32 hit_index = get_hit_index(tag);

    if (hit_index < 0) {
        return MesiState::INVALID;
    }

    return mesi_states[hit_index].get_state();
}

void CacheLine::update_mesi_state(u32 memory, MesiState new_mesi_state)
{
    u32 tag       = parse_tag(memory);
    i32 hit_index = get_hit_index(tag);

    if (hit_index >= 0) {
        mesi_states[hit_index].set_state(new_mesi_state);
        return;
    }
}

u32 CacheLine::parse_tag(u32 address)
{
    u32 tag = address >> (cache_offset + block_offset);
    return tag;
}

u32 CacheLine::get_hit_index(u32 tag)
{
    for (u32 i = 0; i < associativity; i++) {
        if (mesi_states[i].is_valid() && tags[i] == tag) {
            return i;
        }
    }

    return -1;
}