#include "core.hpp"
#include "types.hpp"

#include <fstream>
#include <iostream>
#include <format>

Event Core::step()
{
    if (!input_stream.is_open()) {
        std::cout << std::format("Input file for core {} does not exist!\n", core_idx);
        return Event(core_idx, BusSignal::NOP, -1);
    }

    std::string line;
    if (!std::getline(input_stream, line)) {
        return Event(core_idx, BusSignal::NOP, -1);
    }

    auto [label, value] = parse_line(line);
    auto operation      = parse_core_operation(label);

    if (operation == CoreOperation::OTHER) {
        increase_cycle(value);
        stats->increase_compute_cycle(value);
        return Event(core_idx, BusSignal::NOP, value);
    }

    // check if it is cache write
    bool              is_store            = operation == CoreOperation::STORE;
    CacheAccessResult cache_access_result = cache.access(value, current_cycle);
    increase_cycle(1);

    if (!cache_access_result.cache_hit) {
        stats->cache_miss_activity(block_size);
        // cache miss
        // if it is cache write, send BusRdX to invalidate other cores
        // if it is cache read, send BusRd to fetch data from other cores
        auto bus_signal = is_store ? BusSignal::BusRdX : BusSignal::BusRd;

        return Event(core_idx, bus_signal, value);
    }

    stats->cache_hit_activity();

    if (cache_access_result.mesi_state == MesiState::SHARED) {
        stats->increase_shared_data_access(1);
    } else {
        stats->increase_private_data_access(1);
    }

    // cache hit and cache write operation
    if (is_store) {
        switch (cache_access_result.mesi_state) {
            case MesiState::MODIFIED:
                // M -> M
                break;
            case MesiState::EXCLUSIVE:
                // E -> M
                cache.update_cache_line_mesi_state(value, MesiState::MODIFIED);
                break;
            case MesiState::SHARED:
                // S -> M
                // send BusRdX to invalidate other cores
                cache.update_cache_line_mesi_state(value, MesiState::MODIFIED);
                return Event(core_idx, BusSignal::BusRdX, value);
            default:
                break;
        }
    }

    return Event(core_idx, BusSignal::NOP, value);
}

void Core::stall_until(u64 cycle)
{
    stats->increase_idle_cycle(cycle - current_cycle);
    current_cycle = cycle;
}

MesiState Core::get_cache_line_mesi_state(u32 memory_address)
{
    return cache.get_cache_line_mesi_state(memory_address);
}

void Core::update_cache_line_mesi_state(u32 memory_address, MesiState new_mesi_state)
{
    cache.update_cache_line_mesi_state(memory_address, new_mesi_state);
}

bool Core::fill_cache_line(u32 memory_address, MesiState new_mesi_state)
{
    return cache.fill_cache_line(memory_address, current_cycle, new_mesi_state);
}

/*
 * Each line is in format:
 * <label> <value>
 */
std::pair<u32, u32> Core::parse_line(std::string line)
{
    u32 label = line[0] - '0';
    u32 value = stoul(line.substr(2, line.size() - 2), nullptr, 16);

    return {label, value};
}

CoreOperation Core::parse_core_operation(u32 label)
{
    switch (label) {
        case 0:
            return CoreOperation::LOAD;
        case 1:
            return CoreOperation::STORE;
        default:
            break;
    }

    return CoreOperation::OTHER;
}

void Core::increase_cycle(u32 cycle) { current_cycle += cycle; }