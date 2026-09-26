#include "stats.hpp"

#include <print>

void Stats::print() {
    u32 total_cycle = compute_cycle + load_store_cycle + idle_cycle;

    std::print("Core {} stats print!\n", core_idx);
    std::print("Total cycle: {}, compute cycle: {}, load/store cycle: {}, idle cycle: {}\n",
               total_cycle, compute_cycle, load_store_cycle, idle_cycle);
    std::print("Cache access count: {}, cache hit count: {}, cache miss count: {}\n",
               cache_access_count, cache_hit_count, cache_miss_count);
    std::print("Bus data traffic: {}\n", bus_data_traffic);
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
