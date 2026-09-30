#ifndef STATS_HPP
#define STATS_HPP

#include "types.hpp"

class Stats {
public:
    Stats() = default;
    Stats(u32 core_idx)
        : core_idx(core_idx), compute_cycle(0), load_store_cycle(0), idle_cycle(0),
          cache_access_count(0), cache_hit_count(0), cache_miss_count(0), bus_data_traffic(0),
          bus_update_count(0), bus_invalidation_count(0), private_data_access_count(0),
          shared_data_access_count(0) {}

    void print();
    void increase_compute_cycle(u32 cycle);
    void increase_load_store_cycle(u32 cycle);
    void increase_idle_cycle(u32 cycle);

    void increase_cache_access(u32 count);
    void increase_cache_hit(u32 count);
    void increase_cache_miss(u32 count);

    void increase_bus_traffic(u32 data_bytes);
    void increase_bus_update(u32 count);
    void increase_bus_invalidation(u32 count);

    void increase_private_data_access(u32 count);
    void increase_shared_data_access(u32 count);

private:
    u32 core_idx;
    u64 compute_cycle;
    u64 load_store_cycle;
    u64 idle_cycle;

    u64 cache_access_count;
    u64 cache_hit_count;
    u64 cache_miss_count;

    u64 bus_data_traffic;
    u64 bus_update_count;
    u64 bus_invalidation_count;

    u64 private_data_access_count;
    u64 shared_data_access_count;
};

#endif // STATS_HPP
