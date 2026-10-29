#include "simulator.hpp"
#include "core.hpp"
#include "params.hpp"

#include <iostream>
#include <filesystem>
#include <format>

void Simulator::run()
{
    std::string input_file{config.input_file};

    for (u32 i = 0; i < num_of_cores; i++) {
        std::string input_file_path = std::format("{}_{}.data", input_file, i);

        if (!std::filesystem::exists(input_file_path)) {
            std::cout << std::format("Input file: {} does not exist!\n", input_file_path);
            return;
        }

        cores.emplace_back(i, input_file_path, config.cache_size, config.associativity,
                           config.block_size, stats);
    }

    while (true) {
        Core *core = nullptr;

        // find the core with the smallest current cycle
        for (auto &c : cores) {
            if (!c.is_finished() &&
                (core == nullptr || c.get_current_cycle() < core->get_current_cycle())) {
                core = &c;
            }
        }

        if (core == nullptr) {
            break;
        }

        auto event = core->step();
        handle_bus_event(event);
    }

    stats->print_all_stats();
}

void Simulator::handle_bus_event(const Event &event)
{
    Core     &core           = cores[event.core_idx];
    BusSignal bus_signal     = event.bus_signal;
    u32       memory_address = static_cast<u32>(event.address);

    if (bus_signal == BusSignal::NOP) {
        return;
    }

    u64 start   = std::max(core.get_current_cycle(), bus_free_time);
    u64 latency = 0;

    bool      is_shared      = false;
    MesiState new_mesi_state = MesiState::INVALID;

    switch (bus_signal) {
        case BusSignal::BusRd:
            for (u32 i = 0; i < cores.size(); i++) {
                if (i == static_cast<u32>(event.core_idx) ||
                    cores[i].get_cache_line_mesi_state(memory_address) == MesiState::INVALID) {
                    continue;
                }

                // if other cache line has the same memory address and is not invalid
                is_shared = true;
                cores[i].update_cache_line_mesi_state(memory_address, MesiState::SHARED);
            }

            // if other core have same cache, mesi_state = shared, else exclusive
            new_mesi_state = is_shared ? MesiState::SHARED : MesiState::EXCLUSIVE;
            break;
        case BusSignal::BusRdX:
            for (u32 i = 0; i < cores.size(); i++) {
                if (i == static_cast<u32>(event.core_idx) ||
                    cores[i].get_cache_line_mesi_state(memory_address) == MesiState::INVALID) {
                    continue;
                }

                // if other cache line has the same memory address and is not invalid
                is_shared = true;
                // invalidate other cores cache line
                cores[i].update_cache_line_mesi_state(memory_address, MesiState::INVALID);
                stats->increase_bus_invalidation(1);
            }

            new_mesi_state = MesiState::MODIFIED;
            break;
        default:
            // BusFlush just write back to memory, in this simulator dont need to flush to other cores
            break;
    }

    // handle cache miss
    if (core.get_cache_line_mesi_state(memory_address) == MesiState::INVALID) {
        // if other cores have the same cache line, then cache to cache latency, else cache miss penalty
        latency = is_shared ? CACHE_TO_CACHE_LATENCY(config.block_size) : CACHE_MISS_PENALTY;

        // fill the cache line with new mesi state
        // if it is in modified state, flush evicted cache line to memory (writeback policy)
        if (core.fill_cache_line(memory_address, new_mesi_state)) {
            // need to flush evicted cache into memory
            latency += CACHE_MISS_PENALTY;
            stats->increase_bus_traffic(config.block_size);
        }
    }

    bus_free_time = start + latency;
    core.stall_until(bus_free_time);
}