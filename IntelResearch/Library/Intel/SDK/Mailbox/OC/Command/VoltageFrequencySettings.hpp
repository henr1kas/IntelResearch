#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

// param1 = domain id([0, 4]). TODO: maybe 6
struct VoltageFrequencySettings : OCMailbox::Command<VoltageFrequencySettings, 0x10, 0x11> {
    std::uint32_t maxOcRatio : 8 = 0;
    std::uint32_t voltageTargetU12 : 12 = 0;
    std::uint32_t targetMode : 1 = 0;
    std::uint32_t voltageOffsetS11 : 11 = 0;

    constexpr std::int16_t GetConvertedVoltageOffset() const noexcept {
        return static_cast<std::int16_t>(voltageOffsetS11) - ((voltageOffsetS11 & 1024) << 1);
    }

    constexpr void SetConvertedVoltageOffset(const std::int16_t input) noexcept {
        voltageOffsetS11 = static_cast<std::uint16_t>(input + (input < 0) * (1 << 11));
    }

    constexpr static double GetAccurateVoltage(const std::int16_t input) noexcept {
        return 0.9765625 * input;
    }
};
