#include "core.hpp"

#include <print>

void Core::run() { std::print("Core {} is running!\n", core_idx); }

/*
 * Each line is in format:
 * <label> <value>
 */
std::pair<u32, u32> Core::parse_line(std::string line) {
    u32 label = line[0] - '0';
    u32 value = stoul(line.substr(2, line.size() - 2), nullptr, 16);

    return {label, value};
}
