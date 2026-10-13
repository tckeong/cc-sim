#ifndef MESI_HPP
#define MESI_HPP

#include "directory.hpp"

class MESI {
public:
    enum class State { MODIFIED, EXCLUSIVE, SHARED, INVALID };

    MESI() : state(State::INVALID) {}

    bool is_valid() const { return state != State::INVALID; }

private:
    State     state;
    Directory directory;
};

#endif // MESI_HPP