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

enum class CacheOperation { READ, WRITE };

struct CacheAccessResult {
    bool cache_hit;
    bool bus_update;
};

enum ProcessorSignal { PROCESSOR_READ, PROCESSOR_WRITE };

enum BusSignal { BUS_READ, BUS_READ_EXCLUSIVE, BUS_READX, NONE };

enum MesiState { M, E, S, I };

#endif // TYPES_H
