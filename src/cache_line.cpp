#include "cache_line.hpp"
#include "core.hpp"
#include "mesi.hpp"
#include "types.hpp"

CacheAccessResult CacheLine::cache_read(u32 address, u64 current_cycle)
{
    u32 tag = parse_tag(address);
    i32 hit_index = check_cache_hit(tag, current_cycle);

    if (hit_index >= 0)
    {
        // cache coherence state will transition based on the MESI protocol
        // if found cache, MESI state should be M, E, S, and read will not change states and increase signal
        return CacheAccessResult(true, false);
    }

    // If not found, the MESI state should be I, and read will change the state
    auto [lru_index, bus_update] = resolve_cache_miss(address, current_cycle, CacheOperation::READ);
    return CacheAccessResult(false, bus_update);
}

CacheAccessResult CacheLine::cache_write(u32 address, u64 current_cycle)
{
    u32 tag = parse_tag(address);
    i32 hit_index = check_cache_hit(tag, current_cycle);

    if (hit_index >= 0)
    {
        // cache coherence state will transition based on the MESI protocol
        cache_coherence_states[hit_index]->update_state(directory, address, CacheOperation::WRITE, core_idx);
        // dirty_bits[hit_index] = true;
        return CacheAccessResult(true, false);
    }

    auto [lru_index, bus_update] = resolve_cache_miss(address, current_cycle, CacheOperation::WRITE);
    // dirty_bits[lru_index] = true;
    return CacheAccessResult(false, bus_update);
}

u32 CacheLine::parse_tag(u32 address)
{
    u32 tag = address >> (cache_offset + block_offset);
    return tag;
}

i32 CacheLine::check_cache_hit(u32 tag, u64 current_cycle)
{
    for (u32 i = 0; i < associativity; i++)
    {
        // if (valid_bits[i] && tags[i] == tag) {
        //     last_visit[i] = current_cycle;
        //     return i;
        // }
        if (cache_coherence_states[i]->is_valid() && tags[i] == tag)
        {
            last_visit[i] = current_cycle;
            return i;
        }
    }

    return -1;
}

std::pair<u32, bool> CacheLine::resolve_cache_miss(u32 memory_address, u64 current_cycle, CacheOperation operation)
{
    u32 lru_index = 0;
    u64 lru_cycle = last_visit[0];
    u32 tag = parse_tag(memory_address);

    for (u32 i = 1; i < associativity; i++)
    {
        // if (last_visit[i] < lru_cycle || !valid_bits[i]) {
        //     lru_index = i;
        //     lru_cycle = last_visit[i];
        // }
        if (last_visit[i] < lru_cycle || !cache_coherence_states[i]->is_valid())
        {
            lru_index = i;
            lru_cycle = last_visit[i];
        }
    }

    // This is used to check if the cache line being replaced is dirty and needs to be written back to memory
    // if the lru_index is dirty, we need to update the bus and write back to memory, and make it Invalidate state
    if (cache_coherence_states[lru_index]->get_state() == State::MODIFIED)
    {
        cache_coherence_states[lru_index]->fetch_and_invalidate();
    }
    else
    {
        cache_coherence_states[lru_index]->invalidate();
    }

    // bool bus_update = dirty_bits[lru_index];
    bool bus_update = cache_coherence_states[lru_index]->get_state() == State::MODIFIED;

    // TODO: update the cache MESI state later based on the MESI protocol
    cache_coherence_states[lru_index]->update_state(directory, memory_address, operation, core_idx);
    // valid_bits[lru_index] = true;
    // dirty_bits[lru_index] = false;

    tags[lru_index] = tag;
    last_visit[lru_index] = current_cycle;

    return {lru_index, bus_update};
}