#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PackagePowerSKU : MSR::Register<PackagePowerSKU, 0x614> {
    std::uint64_t packageTdp : 15 = 0;
    std::uint64_t reserved1 : 17 = 0;
    std::uint64_t packageMaxPower : 15 = 0;
    std::uint64_t reserved2 : 17 = 0;
};
