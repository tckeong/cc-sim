#ifndef STATS_HPP
#define STATS_HPP

#include "types.hpp"

class Stats
{
public:
    Stats(u32 num_of_cores)
        : compute_cycle(num_of_cores, 0),
          load_store_cycle(num_of_cores, 0),
          idle_cycle(num_of_cores, 0),
          cache_access_count(num_of_cores, 0),
          cache_hit_count(num_of_cores, 0),
          cache_miss_count(num_of_cores, 0),
          private_data_access_count(num_of_cores, 0),
          shared_data_access_count(num_of_cores, 0) {};

    void print_all_stats();

    void print(u32 core_idx);
    void increase_compute_cycle(u32 core_idx, u32 cycle);
    void increase_load_store_cycle(u32 core_idx, u32 cycle);
    void increase_idle_cycle(u32 core_idx, u32 cycle);

    void increase_cache_access(u32 core_idx, u32 count);
    void increase_cache_hit(u32 core_idx, u32 count);
    void increase_cache_miss(u32 core_idx, u32 count);

    void increase_bus_traffic(u32 data_bytes);
    void increase_bus_update(u32 count);
    void increase_bus_invalidation(u32 count);

    void increase_private_data_access(u32 core_idx, u32 count);
    void increase_shared_data_access(u32 core_idx, u32 count);

    void cache_hit_activity(u32 core_idx);
    void cache_miss_activity(u32 core_idx, u32 block_size);

private:
    std::vector<u64> compute_cycle;
    std::vector<u64> load_store_cycle;
    std::vector<u64> idle_cycle;

    std::vector<u64> cache_access_count;
    std::vector<u64> cache_hit_count;
    std::vector<u64> cache_miss_count;

    u64 bus_data_traffic       = 0;
    u64 bus_update_count       = 0;
    u64 bus_invalidation_count = 0;

    std::vector<u64> private_data_access_count;
    std::vector<u64> shared_data_access_count;

    void print_all_execution_cycle();
    void print_all_compute_cycle();
    void print_all_load_store_cycle();
    void print_all_idle_cycle();
    void print_all_cache_hit_miss_stats();
    void print_all_bus_traffic_stats();
    void print_all_bus_invalidation_update_stats();
    void print_all_private_shared_data_access_stats();
};

#endif // STATS_HPP
