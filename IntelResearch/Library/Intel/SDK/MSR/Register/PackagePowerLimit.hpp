#pragma once

#include "../../../PowerLimit.hpp"
#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PackagePowerLimit : MSR::Register<PackagePowerLimit, 0x610> {
    PowerLimit pl1;
    PowerLimit pl2;
};
