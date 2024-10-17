#pragma once

#include "../../../../Impl/MSR.hpp"
#include "../../../PowerLimit.hpp"
#include <cstdint>

struct PL3Control : MSR::Register<PL3Control, 0x615> {
    PowerLimit pl3;
    PowerLimit reserved;
};
