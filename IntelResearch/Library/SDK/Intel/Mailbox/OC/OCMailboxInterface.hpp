#pragma once

#include <cstdint>

union OCMailboxInterface {
    std::uint32_t interfaceData = 0;
    struct {
        std::uint32_t commandCompletion : 8 = 0;
        std::uint32_t param1 : 8 = 0;
        std::uint32_t param2 : 8 = 0;
        std::uint32_t reserved : 7 = 0;
        std::uint32_t runBusy : 1 = 0;
    } Fields;
};
