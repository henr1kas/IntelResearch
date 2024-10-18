#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

// W.I.P
struct IA32ThermStatus : MSR::Register<IA32ThermStatus, 0x19C> {
    std::uint64_t reserved1 : 16 = 0;
    std::uint64_t currentTemperature : 7 = 0;
    std::uint64_t reserved2 : 41 = 0;
};
