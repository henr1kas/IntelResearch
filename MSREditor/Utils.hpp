#pragma once

#include <cstdint>

#include "Constants.hpp"
#include "OCMailbox.hpp"

namespace Utils {
    inline double UnconvertRoundedOffset(const std::uint64_t y) noexcept {
        const std::int64_t x = y >> 21;
        return x <= 1024 ? static_cast<double>(x) : static_cast<double>(-(2048 - x));
    }

    inline double UnconvertOffset(const std::uint64_t y) noexcept {
        return UnconvertRoundedOffset(y) / 1.024;
    }

    inline double UnpackOffset(const std::uint64_t data) noexcept {
        const std::uint64_t plane = data / (1ull << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        const std::uint64_t offset = data ^ (plane << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        return UnconvertOffset(offset);
    }

    /* wtf is bit 36? */
    /* need add offset as data? */
    inline double GetVoltageOffset(const std::uint8_t plane) noexcept {
        OCMailbox::Write(static_cast<OCMailboxCommand>(plane), (1ull << 36));
        return UnpackOffset(OCMailbox::Read(static_cast<OCMailboxCommand>(plane)));
    }
} // namespace Utils
