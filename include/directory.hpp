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
    void handleWriteMiss(Core core, u32 memoryAddress);
    void handleReadMiss(Core core, u32 memoryAddress);
    void handleWriteBack(Core core, u32 memoryAddress);
    CacheState checkIsExclusive(u32 memoryAddress);

private:
    // memory address -> address cache state
    std::unordered_map<u32, CacheState>             cacheState;
    std::unordered_map<u32, std::vector<u32>>       sharer;
};

#endif // DIRECTORY_HPP
