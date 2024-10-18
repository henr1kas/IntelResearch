#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct TemperatureTarget : MSR::Register<TemperatureTarget, 0x1A2> {
    std::uint64_t tccOffsetTime : 7;
    std::uint64_t tccOffsetClamp : 1;
    std::uint64_t fanTempTarget : 8;
    std::uint64_t maxRefTemp : 8;
    std::uint64_t tccActivationOffset : 6;
    std::uint64_t reserved1 : 1;
    std::uint64_t tccOffsetLock : 1;
    std::uint64_t reserved2 : 32;
};
