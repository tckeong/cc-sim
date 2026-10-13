#ifndef DIRECTORY_HPP
#define DIRECTORY_HPP

#include "types.hpp"

#include <unordered_map>

class Directory {
private:
    enum class State { Uncached, Exclusive, Shared };

    std::unordered_map<u32, State>            state;
    std::unordered_map<u32, std::vector<u32>> sharer;
};

#endif // DIRECTORY_HPP
