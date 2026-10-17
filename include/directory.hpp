#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "types.hpp"
#include "stats.hpp"

#include <unordered_map>

class MESI;

enum class CacheState
{
    Uncached,
    Exclusive,
    Shared
};

// TODO: might need to set up a global directory in simulator.cpp and pass it to each core, so that each core can access the same directory

class Directory {
public:
    Directory(std::vector<Stats> &stats) : stats(stats) {
        this->cacheState = std::unordered_map<u32, CacheState>();
        this->sharer = std::unordered_map<u32, std::vector<u32>>();
    }

    void handleWriteMiss(u32 core_idx, u32 memoryAddress, MESI &mesi_state);
    void handleReadMiss(u32 core_idx, u32 memoryAddress, MESI &mesi_state);
    void handleWriteBack(u32 core_idx, u32 memoryAddress, MESI &mesi_state);
    bool checkIsExclusive(u32 memoryAddress);

private:
    // memory address -> address cache state
    std::unordered_map<u32, CacheState>             cacheState;
    std::unordered_map<u32, std::vector<u32>>       sharer;
    std::vector<Stats>                              &stats;
};

#endif // DIRECTORY_HPP
