#pragma once

#include "../MSR.hpp"

#include <cstdint>

struct RingRatioLimit {
    std::uint64_t ringMaxOcRatio : 7;
    std::uint64_t reserved1 : 1;
    std::uint64_t ringMinOcRatio : 7;
    std::uint64_t reserved2 : 49;

    static constexpr std::uint32_t MSR_RING_RATIO_LIMIT = 0x620;

    static RingRatioLimit Read() noexcept {
        const std::uint64_t ringRatioLimit = MSR::Read(MSR_RING_RATIO_LIMIT);
        return *reinterpret_cast<const RingRatioLimit*>(&ringRatioLimit);
    }

    static void Write(const RingRatioLimit ringRatioLimit) noexcept {
        MSR::Write(MSR_RING_RATIO_LIMIT, *reinterpret_cast<const std::uint64_t*>(&ringRatioLimit));
    }
};
