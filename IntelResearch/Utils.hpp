#pragma once

#include <cstdint>

#include "Constants.hpp"
#include "OCMailbox.hpp"

namespace Utils {
    constexpr std::uint32_t MILLIVOLTS_PER_VOLT = 1000;

    constexpr std::uint16_t ConvertToFixedPointVolts(std::uint16_t input, const bool s11 = false) noexcept {
        constexpr std::uint16_t MAX_TARGET_MV = 4095;
        constexpr std::uint16_t MAX_OFFSET_MV = 500;
        constexpr std::uint16_t INT16_SIGN_BIT_MASK = 0x8000;

        const bool negativeCheck = (input & INT16_SIGN_BIT_MASK) && s11;

        if (negativeCheck)
            input = (~input + 1) & (INT16_SIGN_BIT_MASK - 1);

        if (s11)
            input = std::min(input, MAX_OFFSET_MV);

        const std::uint32_t voltage = 1024u * std::min(input, MAX_TARGET_MV);
        const std::uint32_t remainder = voltage % MILLIVOLTS_PER_VOLT;
        input = static_cast<std::uint16_t>(voltage / MILLIVOLTS_PER_VOLT) + (remainder >= 500u);

        if (negativeCheck)
            input = (~input + 1);

        return input;
    }

    constexpr auto ConvertToBinaryMillivolt(std::uint16_t input, const bool s11 = false) noexcept {
        constexpr std::uint16_t FIXED_POINT_SIGN_BIT_MASK = 0x0400;

        const bool negativeCheck = (input & FIXED_POINT_SIGN_BIT_MASK) && s11;

        if (negativeCheck)
            input = (~input + 1) & (FIXED_POINT_SIGN_BIT_MASK - 1);

        const std::uint32_t voltage = MILLIVOLTS_PER_VOLT * input;
        const std::uint32_t remainder = voltage % 1024u;
        input = static_cast<std::uint16_t>(voltage / 1024u) + (remainder >= 500u);

        if (negativeCheck)
            input = (~input + 1);

        return input;
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
