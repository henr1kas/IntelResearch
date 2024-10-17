#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct TemperatureTarget : MSR::Register<TemperatureTarget, 0x1A2> {
    std::uint64_t reserved1 : 16 = 0;
    std::uint64_t temperatureTarget : 8 = 0;
    std::uint64_t reserved2 : 40 = 0;
};
