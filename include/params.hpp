#ifndef PARAMS_H
#define PARAMS_H

#define DEFAULT_CACHE_SIZE    4 * 1024 // 4 KiB
#define DEFAULT_ASSOCIATIVITY 2
#define DEFAULT_BLOCK_SIZE    32  // 32 bytes
#define WORD_SIZE             4   //  4 bytes
#define CACHE_MISS_PENALTY    100 // cache miss penalty 100 cycles

#define CACHE_TO_CACHE_LATENCY(BLOCK_SIZE) (2 * (BLOCK_SIZE / WORD_SIZE))

#endif // PARAMS_H
