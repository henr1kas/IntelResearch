#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct TurboRatioLimit : MSR::Register<TurboRatioLimit, 0x1AD> {
    std::uint8_t limit[8]; // index = coreCount - 1;
};
