#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "config.hpp"
#include "types.hpp"
#include "core.hpp"
#include "stats.hpp"

class Simulator
{
public:
    Simulator(Config config, u32 num_of_cores)
        : num_of_cores(num_of_cores),
          config(config),
          stats(std::make_shared<Stats>(num_of_cores)),
          cores() {};

    void run();

private:
    u32                    num_of_cores;
    Config                 config;
    std::shared_ptr<Stats> stats;
    std::vector<Core>      cores;

    u64 bus_free_time = 0;

    void handle_bus_event(const Event &event);
};

#endif // SIMULATOR_HPP
