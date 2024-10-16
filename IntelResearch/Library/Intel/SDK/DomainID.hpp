#pragma once

#include <cstdint>

enum class DomainID : std::uint8_t {
    DDR = 0,
    IACORE = 0,
    GTSLICE = 1,
    RING = 2,
    GTUNSLICE = 3,
    SYSTEMAGENT = 4,
    MEMORYCONTROLLER = 6
};
