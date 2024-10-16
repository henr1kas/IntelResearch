#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct RingRatioLimit : MSR::Register<RingRatioLimit, 0x620> {
    std::uint64_t ringMaxOcRatio : 7 = 0;
    std::uint64_t reserved1 : 1 = 0;
    std::uint64_t ringMinOcRatio : 7 = 0;
    std::uint64_t reserved2 : 49 = 0;
};
