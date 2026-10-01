#include "simulator.hpp"
#include "core.hpp"

#include <iostream>
#include <filesystem>
#include <format>
#include <thread>

void Simulator::run() {
    std::string input_file{config.input_file};

    for (u32 i = 0; i < cores.size(); i++) {
        std::string input_file_path = std::format("{}_{}.data", input_file, i);

        if (!std::filesystem::exists(input_file_path)) {
            std::cout << std::format("Input file: {} does not exist!\n", input_file_path);
            return;
        }

        stats[i] = Stats(i);
        cores[i] =
            Core(i, input_file_path, config.cache_size, config.associativity, config.block_size);
    }

    std::vector<std::thread> threads;

    for (u32 i = 0; i < cores.size(); i++) {
        threads.emplace_back(run_core, std::ref(cores[i]), std::ref(stats[i]));
    }

    for (auto &thread : threads) {
        thread.join();
    }

    print_execution_cycle();
    print_compute_cycle();
    print_load_store_cycle();
    print_idle_cycle();
    print_cache_hit_miss_stats();
    print_bus_traffic_stats();
}

void Simulator::print_execution_cycle() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_total_cycle();
    }

    std::cout << std::endl;
}

void Simulator::print_compute_cycle() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_compute_cycle();
    }

    std::cout << std::endl;
}

void Simulator::print_load_store_cycle() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_load_store_cycle();
    }

    std::cout << std::endl;
}

void Simulator::print_idle_cycle() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_idle_cycle();
    }

    std::cout << std::endl;
}

void Simulator::print_cache_hit_miss_stats() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_cache_hit_count() << ":" << stats[i].get_cache_miss_count();
    }

    std::cout << std::endl;
}

void Simulator::print_bus_traffic_stats() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_bus_data_traffic();
    }

    std::cout << std::endl;
}

void Simulator::print_bus_invalidation_update_stats() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_bus_invalidation_count() + stats[i].get_bus_update_count();
    }

    std::cout << std::endl;
}

void Simulator::print_private_shared_data_access_stats() {
    for (u32 i = 0; i < stats.size(); i++) {
        if (i > 0) {
            std::cout << " ";
        }

        std::cout << stats[i].get_private_data_access_count() << ":"
                  << stats[i].get_shared_data_access_count();
    }

    std::cout << std::endl;
}