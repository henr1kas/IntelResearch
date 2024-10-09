#pragma once

#include <cstdint>

#include "Constants.hpp"
#include "OCMailbox.hpp"

namespace Utils {
    constexpr std::uint32_t MILLIVOLTS_PER_VOLT = 1000;

    constexpr std::uint16_t ConvertToFixedPointVolts(const std::uint16_t input) noexcept {
        constexpr std::uint16_t MAX_TARGET_MV = 4095;

        const std::uint32_t voltage = 1024u * std::min(input, MAX_TARGET_MV);
        const std::uint32_t remainder = voltage % MILLIVOLTS_PER_VOLT;
        return static_cast<std::uint16_t>(voltage / MILLIVOLTS_PER_VOLT) + (remainder >= 500u);
    }

    constexpr std::int16_t ConvertOffsetToFixedPointVolts(std::int16_t input) noexcept {
        constexpr std::int16_t MAX_OFFSET_MV = 500;

        constexpr std::uint16_t INT16_SIGN_BIT_MASK = 0x8000;
        const bool numIsNegative = input & INT16_SIGN_BIT_MASK;
        if (numIsNegative)
            input = (~input + 1) & (INT16_SIGN_BIT_MASK - 1);
        const std::int16_t voltage = ConvertToFixedPointVolts(std::min(input, MAX_OFFSET_MV));
        return numIsNegative ? (~(voltage) + 1) : voltage;
    }

    constexpr std::uint16_t ConvertToBinaryMillivolt(const std::uint16_t input) noexcept {
        const std::uint32_t voltage = MILLIVOLTS_PER_VOLT * input;
        const std::uint32_t remainder = voltage % 1024u;
        return static_cast<std::uint16_t>(voltage / 1024u) + (remainder >= 500u);
    }

    constexpr std::int16_t ConvertOffsetToBinaryMillivolt(std::int16_t input) noexcept {
        constexpr std::uint16_t FIXED_POINT_SIGN_BIT_MASK = 0x0400;
        const bool numIsNegative = input & FIXED_POINT_SIGN_BIT_MASK;
        if (numIsNegative)
            input = (~input + 1) & (FIXED_POINT_SIGN_BIT_MASK - 1);
        const std::int16_t voltage = ConvertToBinaryMillivolt(input);
        return numIsNegative ? (~(voltage) + 1) : voltage;
    }

    struct VoltageFrequencySettings {
        std::uint32_t maxOcRatio : 8;
        std::uint32_t voltageTargetU12 : 12;
        std::uint32_t targetMode : 1;
        std::uint32_t voltageOffsetS11 : 11;
    };

    inline VoltageFrequencySettings GetVfSettings(const std::uint8_t domainId) noexcept {
        const std::uint32_t mailboxData = OCMailbox::Read(static_cast<OCMailboxCommand>(static_cast<std::uint8_t>(OCMailboxCommand::VOLTAGEFREQUENCY0) + domainId));
        return *reinterpret_cast<const VoltageFrequencySettings*>(&mailboxData);
    }

    /*
    inline std::uint8_t ReadTemperatureOffset() noexcept {
        return static_cast<std::uint8_t>((MSR::Read(MSRRegister::MSR1A2) & (127 << 24)) >> 24);
    }
    */

    inline bool GetRingDownBin() noexcept {
        const std::uint64_t mailboxData = OCMailbox::Read(OCMailboxCommand::MISCTURBOCONTROL);
        return (~mailboxData & OCMailbox::MISC_TURBO_RING_DOWNBIN_MASK) >> OCMailbox::MISC_TURBO_RING_DOWNBIN_OFFSET;
    }

    inline void SetRingDownBin(const bool enabled) noexcept {
        const std::uint32_t mailboxData = (~static_cast<std::uint32_t>(enabled) << OCMailbox::MISC_TURBO_RING_DOWNBIN_OFFSET) & OCMailbox::MISC_TURBO_RING_DOWNBIN_MASK;
        OCMailbox::Write(OCMailboxCommand::MISCTURBOCONTROL, mailboxData);
    }
} // namespace Utils
