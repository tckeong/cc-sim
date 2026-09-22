#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "config.hpp"

class Simulator {
public:
    Simulator() {}
    Simulator(Config config) : config(config) {}

    void run();

private:
    Config config;
};

#endif // SIMULATOR_HPP
