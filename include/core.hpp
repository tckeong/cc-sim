#ifndef CORE_HPP
#define CORE_HPP

#include <fstream>
#include <memory>

#include "types.hpp"
#include "cache.hpp"
#include "stats.hpp"

enum class CoreOperation { LOAD, STORE, OTHER };

class Core
{
public:
    Core() = default;
    Core(u32 core_idx, std::string input_file, u32 cache_size, u32 associativity, u32 block_size,
         std::shared_ptr<Stats> stats)
        : core_idx(core_idx),
          current_cycle(0),
          block_size(block_size),
          input_stream(input_file),
          stats(stats),
          cache(Cache(cache_size, associativity, block_size, core_idx)) {};

    Event step();
    bool  is_finished() const { return input_stream.eof(); }
    u64   get_current_cycle() const { return current_cycle; }

    // stall the core until the given cycle
    void stall_until(u64 cycle);

    MesiState get_cache_line_mesi_state(u32 memory_address);
    void      update_cache_line_mesi_state(u32 memory_address, MesiState new_mesi_state);

    bool fill_cache_line(u32 memory_address, MesiState new_mesi_state);

private:
    u32                    core_idx;
    u64                    current_cycle;
    u32                    block_size;
    std::ifstream          input_stream;
    std::shared_ptr<Stats> stats;

    Cache cache;

    std::pair<u32, u32> parse_line(std::string line);
    CoreOperation       parse_core_operation(u32 label);
    void                increase_cycle(u32 cycle);
};

#endif // CORE_HPP
