#ifndef CACHE_LINE_HPP
#define CACHE_LINE_HPP

#include "types.hpp"
#include "mesi.hpp"
#include "cache_coherence_model.hpp"

#include <cmath>
#include <memory>

class CacheLine {
public:
    CacheLine() = default;
    CacheLine(u32 cache_offset, u32 associativity, u32 block_size)
        : cache_offset(cache_offset), associativity(associativity), block_size(block_size),
        //   valid_bits(associativity, false), dirty_bits(associativity, false),
          tags(associativity, 0), last_visit(associativity, 0),
          cache_coherence_states(associativity) {
        block_offset = std::log2(block_size);
        for (u32 i = 0; i < associativity; i++) {
            cache_coherence_states[i] = std::make_unique<MESI>();
        }
    };

    CacheAccessResult cache_read(u32 address, u64 current_cycle);
    CacheAccessResult cache_write(u32 address, u64 current_cycle);

private:
    u32 cache_offset;
    u32 associativity;
    u32 block_size;
    u32 block_offset;
    u32 core_idx;

    // valid bit and dirty bit are replaced by the MESI state for each cache line
    // std::vector<bool>                                 valid_bits;
    // std::vector<bool>                                 dirty_bits;
    std::vector<u32>                                  tags;
    std::vector<u64>                                  last_visit;
    // Each cache line has a unique MESI state for each way in the set
    std::vector<std::unique_ptr<MESI>> cache_coherence_states;

    u32                  parse_tag(u32 address);
    i32                  check_cache_hit(u32 tag, u64 current_cycle);
    std::pair<u32, bool> resolve_cache_miss(u32 memory_address, u64 current_cycle, CacheOperation operation, u32 core_idx);
};

#endif // CACHE_LINE_HPP
