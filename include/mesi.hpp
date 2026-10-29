#ifndef MESI_HPP
#define MESI_HPP

#include "types.hpp"

class MESI
{
public:
    MESI()
        : state(MesiState::INVALID) {};

    MESI(MesiState initial_state)
        : state(initial_state) {};

    // Additional methods for MESI protocol
    MesiState get_state() const { return state; }

    bool is_read_valid() const { return state != MesiState::INVALID; }
    bool is_write_valid() const
    {
        return state == MesiState::MODIFIED || state == MesiState::EXCLUSIVE;
    }
    bool is_valid() const { return state != MesiState::INVALID; }

    void invalidate() { state = MesiState::INVALID; }
    void set_state(MesiState new_state) { state = new_state; }

private:
    MesiState state;
};

#endif // MESI_HPP