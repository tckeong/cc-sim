#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "config.hpp"
#include "types.h"
#include "core.hpp"

class Simulator {
public:
    Simulator() {}
    Simulator(Config config, u32 num_of_cores) : config(config), cores(num_of_cores) {}

    void run();

private:
    Config            config;
    std::vector<Core> cores;

    static void run_core(Core &core) { core.run(); }
};

#endif // SIMULATOR_HPP
