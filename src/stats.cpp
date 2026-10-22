#include "stats.hpp"
#include "params.hpp"

#include <iostream>
#include <format>

void Stats::print(u32 core_idx)
{
    u64 total_cycle = compute_cycle[core_idx] + load_store_cycle[core_idx] + idle_cycle[core_idx];

    std::cout << std::format("==================== Core {} ====================\n", core_idx);
    std::cout << std::format("Total cycle: {}\n", total_cycle);
    std::cout << std::format("Compute cycle: {}\n", compute_cycle[core_idx]);
    std::cout << std::format("Load/Store cycle: {}\n", load_store_cycle[core_idx]);
    std::cout << std::format("Idle cycle: {}\n", idle_cycle[core_idx]);
    std::cout << std::format("Cache access count: {}\n", cache_access_count[core_idx]);
    std::cout << std::format("Cache hit count: {}\n", cache_hit_count[core_idx]);
    std::cout << std::format("Cache miss count: {}\n", cache_miss_count[core_idx]);
    std::cout << std::format("Bus data traffic (bytes): {}\n", bus_data_traffic);
    std::cout << std::format("=================================================\n", core_idx);
}

void Stats::increase_compute_cycle(u32 core_idx, u32 cycle)
{
    this->compute_cycle[core_idx] += cycle;
}
void Stats::increase_load_store_cycle(u32 core_idx, u32 cycle)
{
    this->load_store_cycle[core_idx] += cycle;
}
void Stats::increase_idle_cycle(u32 core_idx, u32 cycle) { this->idle_cycle[core_idx] += cycle; }

void Stats::increase_cache_access(u32 core_idx, u32 count)
{
    this->cache_access_count[core_idx] += count;
}
void Stats::increase_cache_hit(u32 core_idx, u32 count)
{
    this->cache_hit_count[core_idx] += count;
}
void Stats::increase_cache_miss(u32 core_idx, u32 count)
{
    this->cache_miss_count[core_idx] += count;
}

void Stats::increase_bus_traffic(u32 data_bytes) { this->bus_data_traffic += data_bytes; }
void Stats::increase_bus_update(u32 count) { this->bus_update_count += count; }
void Stats::increase_bus_invalidation(u32 count) { this->bus_invalidation_count += count; }

void Stats::increase_private_data_access(u32 core_idx, u32 count)
{
    this->private_data_access_count[core_idx] += count;
}
void Stats::increase_shared_data_access(u32 core_idx, u32 count)
{
    this->shared_data_access_count[core_idx] += count;
}

void Stats::cache_hit_activity(u32 core_idx)
{
    this->load_store_cycle[core_idx]++;
    this->cache_access_count[core_idx]++;
    this->cache_hit_count[core_idx]++;
}

void Stats::cache_miss_activity(u32 core_idx, u32 block_size)
{
    this->load_store_cycle[core_idx]++;
    this->cache_access_count[core_idx]++;
    this->cache_miss_count[core_idx]++;
    this->bus_data_traffic += block_size;
}

void Stats::print_all_stats()
{
    print_all_execution_cycle();
    print_all_compute_cycle();
    print_all_load_store_cycle();
    print_all_idle_cycle();
    print_all_cache_hit_miss_stats();
    print_all_bus_traffic_stats();
    print_all_bus_invalidation_update_stats();
    print_all_private_shared_data_access_stats();
}

void Stats::print_all_execution_cycle()
{
    for (u32 i = 0; i < compute_cycle.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << compute_cycle[i] + load_store_cycle[i] + idle_cycle[i];
    }

    std::cout << std::endl;
}

void Stats::print_all_compute_cycle()
{
    for (u32 i = 0; i < compute_cycle.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << compute_cycle[i];
    }

    std::cout << std::endl;
}

void Stats::print_all_load_store_cycle()
{
    for (u32 i = 0; i < load_store_cycle.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << load_store_cycle[i];
    }

    std::cout << std::endl;
}

void Stats::print_all_idle_cycle()
{
    for (u32 i = 0; i < idle_cycle.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << idle_cycle[i];
    }

    std::cout << std::endl;
}

void Stats::print_all_cache_hit_miss_stats()
{
    for (u32 i = 0; i < cache_hit_count.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << cache_hit_count[i] << ":" << cache_miss_count[i];
    }

    std::cout << std::endl;
}

void Stats::print_all_bus_traffic_stats() { std::cout << bus_data_traffic << std::endl; }
void Stats::print_all_bus_invalidation_update_stats()
{
    std::cout << bus_invalidation_count + bus_update_count << std::endl;
}

void Stats::print_all_private_shared_data_access_stats()
{
    for (u32 i = 0; i < private_data_access_count.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << private_data_access_count[i] << ":" << shared_data_access_count[i];
    }

    std::cout << std::endl;
}