#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct UncoreFrequency : MSR::Register<UncoreFrequency, 0x621> {
    std::uint64_t uncoreFrequency : 7 = 0;
    std::uint64_t reserved : 57 = 0;
};
