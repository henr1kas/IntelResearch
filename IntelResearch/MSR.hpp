#pragma once

#include "Constants.hpp"
#include <cstdint>

#ifdef _WIN32
#include "Windows/Driver.hpp"
#endif

namespace MSR {
    inline void Write(const MSRRegister reg, const std::uint64_t val) noexcept {
#ifdef _WIN32
        Driver::WriteMSR(msrRegisters[static_cast<std::uint64_t>(reg)], val);
#endif
    }

    inline std::uint64_t Read(const MSRRegister reg) noexcept {
#ifdef _WIN32
        return Driver::ReadMSR(msrRegisters[static_cast<std::uint64_t>(reg)]);
#endif
    }

    /* move these idk where */
    constexpr std::uint64_t V_MSR_RING_RATIO_LIMIT_MASK = 0x7F;
    /*
    inline std::uint8_t ReadTemperatureOffset() noexcept {
        return static_cast<std::uint8_t>((MSR::Read(MSRRegister::MSR1A2) & (127 << 24)) >> 24);
    }
    */
    inline void ReadRingRatioLimit(std::uint8_t& ringMaxOcRatio, std::uint8_t& ringMinOcRatio) noexcept {
        const std::uint64_t msrData = MSR::Read(MSRRegister::RINGRATIOLIMIT);
        ringMaxOcRatio = static_cast<std::uint8_t>(msrData & MSR::V_MSR_RING_RATIO_LIMIT_MASK);
        ringMinOcRatio = static_cast<std::uint8_t>((msrData >> 8) & MSR::V_MSR_RING_RATIO_LIMIT_MASK);
    }

    inline void WriteRingRatioLimit(const std::uint8_t ringMaxOcRatio, const std::uint8_t ringMinOcRatio) noexcept {
        std::uint64_t msrData = MSR::Read(MSRRegister::RINGRATIOLIMIT);
        msrData &= ~(MSR::V_MSR_RING_RATIO_LIMIT_MASK);
        msrData |= (ringMaxOcRatio & MSR::V_MSR_RING_RATIO_LIMIT_MASK);
        msrData &= ~(MSR::V_MSR_RING_RATIO_LIMIT_MASK << 8);
        msrData |= (static_cast<std::uint64_t>(ringMinOcRatio & MSR::V_MSR_RING_RATIO_LIMIT_MASK) << 8);
        MSR::Write(MSRRegister::RINGRATIOLIMIT, msrData);
    }
} // namespace MSR
