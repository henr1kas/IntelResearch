#pragma once

#include "Constants.hpp"
#include <cstdint>

#ifdef _WIN32
#include "Windows/Driver.hpp"
#else
#include <fcntl.h>
#endif

namespace MSR {
#ifndef _WIN32
    const std::int32_t cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    inline void Write(const MSRRegister reg, const std::uint64_t val) noexcept {
#ifdef _WIN32
        Driver::WriteMSR(msrRegisters[static_cast<std::uint64_t>(reg)], val);
#else
        for (std::int32_t i = 0; i < cpuCount; ++i) {
            const std::string cpu = "/dev/cpu/" + std::to_string(i) + "/msr";
            int fd = open(cpu.data(), O_WRONLY);
            lseek(fd, msrRegisters[static_cast<std::uint64_t>(reg)], SEEK_SET);
            write(fd, &val, sizeof(val));
            close(fd);
        }
#endif
    }

    inline std::uint64_t Read(const MSRRegister reg) noexcept {
#ifdef _WIN32
        return Driver::ReadMSR(msrRegisters[static_cast<std::uint64_t>(reg)]);
#else
        std::uint64_t val;
        int fd = open("/dev/cpu/0/msr", O_RDONLY);
        lseek(fd, msrRegisters[static_cast<std::uint64_t>(reg)], SEEK_SET);
        read(fd, &val, sizeof(val));
        close(fd);
        return val;
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
