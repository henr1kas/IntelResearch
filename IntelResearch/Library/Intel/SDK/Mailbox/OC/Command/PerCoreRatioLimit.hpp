#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

struct PerCoreRatioLimit : OCMailbox::Command<PerCoreRatioLimit, 0x2, 0x3> {
    std::uint8_t cores[4];
};
