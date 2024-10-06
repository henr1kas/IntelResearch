#pragma once

#include "Constants.hpp"
#include <cstdint>

#ifdef _WIN32
#include "Driver.hpp"
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
} // namespace MSR
