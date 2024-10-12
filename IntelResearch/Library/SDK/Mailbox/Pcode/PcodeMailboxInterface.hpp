#pragma once

#include <cstdint>

union PcodeMailboxInterface {
    std::uint32_t interfaceData = 0;
    struct {
        std::uint32_t command : 8 = 0;
        std::uint32_t address : 21 = 0;
        std::uint32_t reserved : 2 = 0;
        std::uint32_t runBusy : 1 = 0;
    } Fields;
};
