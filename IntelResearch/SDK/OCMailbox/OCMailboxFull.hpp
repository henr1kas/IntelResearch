#pragma once

#include <cstdint>

struct OCMailboxFull {
    std::uint32_t data = 0;
    std::uint8_t commandCompletion = 0;
    std::uint8_t param1 = 0;
    std::uint8_t param2 = 0;
    std::uint8_t reserved : 7 = 0;
    std::uint8_t runBusy : 1 = 0;
};
