#pragma once

#include "../../../../Impl/MSR.hpp"
#include "../../../PowerLimit.hpp"
#include <cstdint>

struct PL4Control : MSR::Register<PL4Control, 0x601> {
    PowerLimit pl4;
    PowerLimit reserved;
};
