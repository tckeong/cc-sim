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
        : config(config),
          cores()
    {
        for (u32 i = 0; i < num_of_cores; i++) {
            stats.emplace_back(std::make_shared<Stats>(i));
        }
    };

    void run();

private:
    Config                              config;
    std::vector<std::shared_ptr<Stats>> stats;
    std::vector<Core>                   cores;

    u64 bus_free_time = 0;

    void print_execution_cycle();
    void print_compute_cycle();
    void print_load_store_cycle();
    void print_idle_cycle();
    void print_cache_hit_miss_stats();
    void print_bus_traffic_stats();
    void print_bus_invalidation_update_stats();
    void print_private_shared_data_access_stats();

    void invalidate_cores_cache_line(const std::vector<u32> &sharers, u32 memory_address);
    void update_cores_cache_line_mesi_state(const std::vector<u32> &sharers, u32 memory_address,
                                            const std::vector<MesiState> &mesi_state);

    void handle_bus_event(const Event &event);
};

#endif // SIMULATOR_HPP
