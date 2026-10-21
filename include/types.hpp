#ifndef TYPES_H
#define TYPES_H

#include <cstdint>
#include <string>
#include <vector>

typedef uint8_t  u8;
typedef int32_t  i32;
typedef uint32_t u32;
typedef int64_t  i64;
typedef uint64_t u64;

enum class MesiState { MODIFIED, EXCLUSIVE, SHARED, INVALID };

struct CacheAccessResult {
    bool      cache_hit;
    MesiState mesi_state;

    CacheAccessResult(bool hit, MesiState mesi)
        : cache_hit(hit),
          mesi_state(mesi) {};
};

enum class BusSignal {
    BusRd,    // bus read
    BusRdX,   // bus read exclusive
    BusFlush, // flush, write back memory
    NOP
};

struct Event {
    i32       core_idx;
    BusSignal bus_signal;
    i64       address;

    Event()
        : core_idx(-1),
          bus_signal(BusSignal::NOP),
          address(0) {};

    Event(i32 idx, BusSignal bus_signal, i64 address)
        : core_idx(idx),
          bus_signal(bus_signal),
          address(address) {};
};

#endif // TYPES_H
