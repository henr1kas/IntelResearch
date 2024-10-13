#pragma once

#include "../MSR.hpp"

#include <cstdint>

struct RingRatioLimit {
    std::uint64_t ringMaxOcRatio : 7 = 0;
    std::uint64_t reserved1 : 1 = 0;
    std::uint64_t ringMinOcRatio : 7 = 0;
    std::uint64_t reserved2 : 49 = 0;

    static constexpr std::uint32_t MSR_RING_RATIO_LIMIT = 0x620;

    static RingRatioLimit Read() noexcept {
        return MSR::Read<RingRatioLimit>(MSR_RING_RATIO_LIMIT);
    }

    static void Write(const RingRatioLimit ringRatioLimit) noexcept {
        MSR::Write(MSR_RING_RATIO_LIMIT, ringRatioLimit);
    }
};
