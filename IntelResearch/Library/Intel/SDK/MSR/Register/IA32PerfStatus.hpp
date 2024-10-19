#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

// W.I.P
struct IA32PerfStatus : MSR::Register<IA32PerfStatus, 0x198> {
    std::uint64_t state : 16 = 0;
    std::uint64_t reserved : 48 = 0;

    std::uint64_t CoreRatio() const noexcept {
        return (state >> 8) & 0xFF;
    }
};
