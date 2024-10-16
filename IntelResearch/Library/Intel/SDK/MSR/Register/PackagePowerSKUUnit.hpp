#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PackagePowerSKUUnit : MSR::Register<PackagePowerSKUUnit, 0x606> {
    std::uint64_t powerUnit : 4 = 0;
    std::uint64_t reserved : 60 = 0;
};
