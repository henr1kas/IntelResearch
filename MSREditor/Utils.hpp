#pragma once

#include <cstdint>

#include "Constants.hpp"
#include "OCMailbox.hpp"

namespace Utils {
    constexpr double VoltageOffsetToDouble(const std::uint64_t offset) noexcept {
        const std::int64_t x = offset >> 21;
        return (x <= 1024 ? static_cast<double>(x) : static_cast<double>(-(2048 - x))) / 1.024;
    }

    constexpr std::uint32_t VoltageOffsetToInteger(const double offset) noexcept {
        const double scaledValue = (offset * 1.024);
        const std::uint32_t x = static_cast<std::uint32_t>(scaledValue + 0.5 - (scaledValue < 0.0));
        return 0xFFE00000 & ((x & 0xFFF) << 21);
    }

    inline std::uint64_t GetVoltageOffset(const std::uint8_t plane) noexcept {
        return OCMailbox::Read(static_cast<OCMailboxCommand>(static_cast<std::uint8_t>(OCMailboxCommand::UNK10_0) + plane));
    }

    inline void SetVoltageOffset(const std::uint8_t plane, const std::uint32_t val) noexcept {
        return OCMailbox::Write(static_cast<OCMailboxCommand>(static_cast<std::uint8_t>(OCMailboxCommand::UNK10_0) + plane), val);
    }

    inline std::uint8_t ReadTemperatureOffset() noexcept {
        return static_cast<std::uint8_t>((MSR::Read(MSRRegister::MSR1A2) & (127 << 24)) >> 24);
    }

    inline std::uint64_t GetCacheRatioOffset() noexcept {
        return OCMailbox::Read(OCMailboxCommand::UNK18_0);
    }

    inline void SetCacheRatioOffset(const std::uint32_t offset) noexcept {
        OCMailbox::Write(OCMailboxCommand::UNK18_0, offset);
    }
} // namespace Utils
