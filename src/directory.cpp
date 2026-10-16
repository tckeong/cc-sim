#include "directory.hpp"
#include "core.hpp"
#include "stats.hpp"
#include "types.hpp"

void Directory::handleWriteMiss(u32 core_idx, u32 memoryAddress, MESI &mesi_state)
{
    // Write miss from the core_idx adds 1 traffic to the bus
    stats.increase_bus_traffic(1);

    CacheState currentState = cacheState[memoryAddress];

    switch (currentState)
    {
    case CacheState::Uncached:
        // Transition to Exclusive state
        cacheState[memoryAddress] = CacheState::Exclusive;
        sharer[memoryAddress].clear();
        sharer[memoryAddress].push_back(core_idx);
        break;

    case CacheState::Exclusive:
        for (u32 sharer_core : sharer[memoryAddress])
        {
            // every single sharer needs to invalidate their cache line, so we need to call fetch_and_invalidate for each sharer
            mesi_state.fetch_and_invalidate();
            stats.increase_bus_traffic(1);
            stats.increase_bus_invalidation(1);
        }

        sharer[memoryAddress].clear();
        sharer[memoryAddress].push_back(core_idx);
        break;

    case CacheState::Shared:
        // Transition to Exclusive state
        cacheState[memoryAddress] = CacheState::Exclusive;
        for (u32 sharer_core : sharer[memoryAddress])
        {
            // every single sharer needs to invalidate their cache line, so we need to call fetch_and_invalidate for each sharer
            mesi_state.fetch_and_invalidate();
            stats.increase_bus_invalidation(1);
        }

        sharer[memoryAddress].clear();
        sharer[memoryAddress].push_back(core_idx);
        break;
    }
}

void Directory::handleReadMiss(u32 core_idx, u32 memoryAddress, MESI &mesi_state)
{
    // Read miss from the core_idx adds 1 traffic to the bus
    stats.increase_bus_traffic(1);

    CacheState currentState = cacheState[memoryAddress];

    switch (currentState)
    {
    case CacheState::Uncached:
        // Transition to Shared state
        cacheState[memoryAddress] = CacheState::Shared;
        sharer[memoryAddress].clear();
        sharer[memoryAddress].push_back(core_idx);
        break;

    case CacheState::Exclusive:
        // Transition to Shared state
        cacheState[memoryAddress] = CacheState::Shared;

        for (u32 sharer_core : sharer[memoryAddress])
        {
            mesi_state.fetch();
            stats.increase_bus_traffic(1);
        }

        sharer[memoryAddress].push_back(core_idx);
        break;

    case CacheState::Shared:
        sharer[memoryAddress].push_back(core_idx);
        break;
    }
}

void Directory::handleWriteBack(u32 core_idx, u32 memoryAddress, MESI &mesi_state)
{
    // Write back from the core_idx adds 1 traffic to the bus
    stats.increase_bus_traffic(1);

    CacheState currentState = cacheState[memoryAddress];

    switch (currentState)
    {
    case CacheState::Uncached:
        // This should not happen, as a write back should only occur when the cache line is in Exclusive or Shared state
        break;

    case CacheState::Exclusive:
        // Transition to Uncached state
        cacheState[memoryAddress] = CacheState::Uncached;
        sharer[memoryAddress].clear();
        break;

    case CacheState::Shared:
        // Transition to Uncached state
        cacheState[memoryAddress] = CacheState::Uncached;
        sharer[memoryAddress].clear();
        break;
    }
}

bool Directory::checkIsExclusive(u32 memoryAddress)
{
    return cacheState[memoryAddress] == CacheState::Exclusive && sharer[memoryAddress].size() == 1;
}