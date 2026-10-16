#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "types.hpp"

#include <unordered_map>
#include "core.hpp"

enum class CacheState
{
    Uncached,
    Exclusive,
    Shared
};

class Directory {
public:
    Directory(Stats &stats) : stats(stats) {
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
    Stats                                           &stats;
};

#endif // DIRECTORY_HPP
