#ifndef CACHE_LINE_HPP
#define CACHE_LINE_HPP

#include "types.hpp"
#include "mesi.hpp"

#include <cmath>

class CacheLine
{
public:
    CacheLine(u32 cache_offset, u32 associativity, u32 block_size, u32 core_idx)
        : cache_offset(cache_offset),
          associativity(associativity),
          block_size(block_size),
          core_idx(core_idx),
          tags(associativity, 0),
          last_visit(associativity, 0),
          mesi_states(associativity, MesiState::INVALID)
    {
        block_offset = std::log2(block_size);
    };

    // lookup the cache line for the given address and return the result
    // Just update last visit time if hit
    CacheAccessResult lookup(u32 address, u64 current_cycle);

    // fill the cache line with new mesi state
    // return true if the replaced cache line is in modified state
    bool fill(u32 address, u64 current_cycle, MesiState new_mesi_state);

    MesiState get_mesi_state(u32 address);
    void      update_mesi_state(u32 address, MesiState new_mesi_state);

private:
    u32 cache_offset;
    u32 associativity;
    u32 block_size;
    u32 block_offset;
    u32 core_idx;

    std::vector<u32>  tags;
    std::vector<u64>  last_visit;
    std::vector<MESI> mesi_states;

    u32 parse_tag(u32 address);
    u32 get_hit_index(u32 tag);
};

#endif // CACHE_LINE_HPP
