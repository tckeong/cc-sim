#include "core.hpp"
#include "types.hpp"

#include <fstream>
#include <iostream>
#include <format>

void Core::run(Stats &stats) {
    std::ifstream input_stream(input_file);

    if (!input_stream.is_open()) {
        std::cout << "None";
        std::cout << std::format("Input file: {} for core {} does not exist!\n", input_file,
                                 core_idx);
        return;
    }

    std::string line;
    while (std::getline(input_stream, line)) {
        auto [label, value] = parse_line(line);
        auto operation      = parse_core_operation(label);

        bool cache_access = (operation == CoreOperation::LOAD || operation == CoreOperation::STORE);
        CacheAccessResult cache_access_result{.cache_hit = false, .bus_update = false};

        switch (operation) {
            case CoreOperation::LOAD:
                cache_access_result = cache.access(CacheOperation::READ, value, current_cycle);
                stats.increase_load_store_cycle(1);
                stats.increase_cache_access(1);
                break;
            case CoreOperation::STORE:
                cache_access_result = cache.access(CacheOperation::WRITE, value, current_cycle);
                stats.increase_load_store_cycle(1);
                stats.increase_cache_access(1);
                break;
            default:
                increase_cycle(value);
                stats.increase_compute_cycle(value);
                break;
        }

        if (cache_access && cache_access_result.cache_hit) {
            increase_cycle(1);
            stats.increase_cache_hit(1);
        } else if (cache_access && !cache_access_result.cache_hit) {
            increase_cycle(1);
            increase_cycle(100);
            stats.increase_bus_traffic(block_size);
            stats.increase_idle_cycle(100);
            stats.increase_cache_miss(1);

            if (cache_access_result.bus_update) {
                increase_cycle(100);
                stats.increase_bus_traffic(block_size);
                stats.increase_idle_cycle(100);
            }
        }
    }
}

/*
 * Each line is in format:
 * <label> <value>
 */
std::pair<u32, u32> Core::parse_line(std::string line) {
    u32 label = line[0] - '0';
    u32 value = stoul(line.substr(2, line.size() - 2), nullptr, 16);

    return {label, value};
}

CoreOperation Core::parse_core_operation(u32 label) {
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