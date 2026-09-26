#include "types.h"
#include "simulator.hpp"
#include "core.hpp"

#include <print>
#include <filesystem>
#include <format>
#include <thread>

void Simulator::run() {
    std::string input_file{config.input_file};

    for (u32 i = 0; i < cores.size(); i++) {
        std::string input_file_path = format("{}_{}.data", input_file, i);

        if (!std::filesystem::exists(input_file_path)) {
            std::print("Input file: {} does not exist!\n", input_file_path);
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

    std::print("Simulation start!\n");

    for (auto &t : threads) {
        t.join();
    }

    for (auto &s : stats) {
        s.print();
    }

    std::print("Simulation end!\n");
}
