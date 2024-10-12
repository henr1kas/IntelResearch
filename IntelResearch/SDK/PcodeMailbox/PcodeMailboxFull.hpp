#pragma once

#include <cstdint>
#include "PcodeMailboxInterface.hpp"

struct PcodeMailboxFull {
    PcodeMailboxInterface Interface;
    std::uint32_t data = 0;
};
