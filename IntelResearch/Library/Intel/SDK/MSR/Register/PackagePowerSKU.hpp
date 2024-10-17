#pragma once

#include "../../../PowerLimit.hpp"
#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PackagePowerSKU : MSR::Register<PackagePowerSKU, 0x614> {
    PowerLimit tdp;
    PowerLimit maxPower;
};
