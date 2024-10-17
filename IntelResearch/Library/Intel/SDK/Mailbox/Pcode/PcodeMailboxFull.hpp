#pragma once

#include "PcodeMailboxInterface.hpp"
#include <cstdint>

template<typename T = std::uint32_t>
struct PcodeMailboxFull {
    PcodeMailboxInterface Interface;
    T data{};
};
