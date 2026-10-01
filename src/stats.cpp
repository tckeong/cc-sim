#include "stats.hpp"

#include <iostream>
#include <format>

void Stats::print() {
    u64 total_cycle = compute_cycle + load_store_cycle + idle_cycle;

    std::cout << std::format("==================== Core {} ====================\n", core_idx);
    std::cout << std::format("Total cycle: {}\n", total_cycle);
    std::cout << std::format("Compute cycle: {}\n", compute_cycle);
    std::cout << std::format("Load/Store cycle: {}\n", load_store_cycle);
    std::cout << std::format("Idle cycle: {}\n", idle_cycle);
    std::cout << std::format("Cache access count: {}\n", cache_access_count);
    std::cout << std::format("Cache hit count: {}\n", cache_hit_count);
    std::cout << std::format("Cache miss count: {}\n", cache_miss_count);
    std::cout << std::format("Bus data traffic (bytes): {}\n", bus_data_traffic);
    std::cout << std::format("=================================================\n", core_idx);
}

void Stats::increase_compute_cycle(u32 cycle) { this->compute_cycle += cycle; }
void Stats::increase_load_store_cycle(u32 cycle) { this->load_store_cycle += cycle; }
void Stats::increase_idle_cycle(u32 cycle) { this->idle_cycle += cycle; }

void Stats::increase_cache_access(u32 count) { this->cache_access_count += count; }
void Stats::increase_cache_hit(u32 count) { this->cache_hit_count += count; }
void Stats::increase_cache_miss(u32 count) { this->cache_miss_count += count; }

void Stats::increase_bus_traffic(u32 data_bytes) { this->bus_data_traffic += data_bytes; }
void Stats::increase_bus_update(u32 count) { this->bus_update_count += count; }
void Stats::increase_bus_invalidation(u32 count) { this->bus_invalidation_count += count; }

void Stats::increase_private_data_access(u32 count) { this->private_data_access_count += count; }
void Stats::increase_shared_data_access(u32 count) { this->shared_data_access_count += count; }

u64 Stats::get_total_cycle() { return compute_cycle + load_store_cycle + idle_cycle; }
u64 Stats::get_compute_cycle() { return compute_cycle; }
u64 Stats::get_load_store_cycle() { return load_store_cycle; }
u64 Stats::get_idle_cycle() { return idle_cycle; }
u64 Stats::get_cache_hit_count() { return cache_hit_count; }
u64 Stats::get_cache_miss_count() { return cache_miss_count; }
u64 Stats::get_bus_data_traffic() { return bus_data_traffic; }
u64 Stats::get_bus_update_count() { return bus_update_count; }
u64 Stats::get_bus_invalidation_count() { return bus_invalidation_count; }
u64 Stats::get_private_data_access_count() { return private_data_access_count; }
u64 Stats::get_shared_data_access_count() { return shared_data_access_count; }
