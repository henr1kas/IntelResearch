#pragma once

#include <cstdint>
#include "OCMailboxInterface.hpp"

template<typename T = std::uint32_t>
struct OCMailboxFull {
    T data{};
    OCMailboxInterface Interface;
};
