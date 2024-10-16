#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

struct MiscTurboControl : OCMailbox::Command<MiscTurboControl, 0x18, 0x19> {
    std::uint32_t reserved1 : 1 = 0;
    std::uint32_t ringDownbin : 1 = 0; // 0 = enabled, 1 = disabled
    std::uint32_t tvbRatio : 1 = 0;
    std::uint32_t tvbVoltage : 1 = 0;
    std::uint32_t reserved2 : 28 = 0;
};
