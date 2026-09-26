#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "config.hpp"
#include "types.hpp"
#include "core.hpp"
#include "stats.hpp"

class Simulator {
public:
    Simulator() {}
    Simulator(Config config, u32 num_of_cores)
        : config(config), stats(num_of_cores), cores(num_of_cores) {}

    void run();

private:
    Config             config;
    std::vector<Stats> stats;
    std::vector<Core>  cores;

    static void run_core(Core &core, Stats &stats) { core.run(stats); }
};

#endif // SIMULATOR_HPP
