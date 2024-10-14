#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PackagePowerLimit {
    std::uint64_t powerLimit1 : 15 = 0;          // POWER_LIMIT_MASK
    std::uint64_t powerLimit1Enable : 1 = 0;     // B_POWER_LIMIT_ENABLE
    std::uint64_t criticalPowerClampPl1 : 1 = 0; // B_CRITICAL_POWER_CLAMP_ENABLE
    std::uint64_t powerLimit1Time : 7 = 0;       // POWER_LIMIT_1_TIME_MASK
    std::uint64_t reserved1 : 8 = 0;             // unused?

    std::uint64_t powerLimit2 : 15 = 0;
    std::uint64_t powerLimit2Enable : 1 = 0;
    std::uint64_t criticalPowerClampPl2 : 1 = 0;
    std::uint64_t powerLimit2Time : 7 = 0;
    std::uint64_t reserved2 : 8 = 0;

    static constexpr std::uint32_t MSR_PACKAGE_POWER_LIMIT = 0x610;

    static PackagePowerLimit Read() noexcept {
        return MSR::Read<PackagePowerLimit>(MSR_PACKAGE_POWER_LIMIT);
    }

    static void Write(const PackagePowerLimit packagePowerLimit) noexcept {
        MSR::Write(MSR_PACKAGE_POWER_LIMIT, packagePowerLimit);
    }
};
