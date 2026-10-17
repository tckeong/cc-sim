#ifndef MESI_HPP
#define MESI_HPP

#include "directory.hpp"

enum class State
{
    MODIFIED,
    EXCLUSIVE,
    SHARED,
    INVALID
};

class MESI
{
public:
    MESI() : state(State::INVALID) {}

    // Additional methods for MESI protocol
    State get_state() const { return state; }
    bool is_valid() const { return state != State::INVALID; }
    void fetch();
    void fetch_and_invalidate();
    void invalidate();
    void update_state(Directory &directory, u32 memory_address, CacheOperation operation, u32 core_idx);

private:
    State state;
};

#endif // MESI_HPP