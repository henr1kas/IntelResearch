#pragma once

#include "../MSR.hpp"

#include <cstdint>

struct PackagePowerSKUUnit {
    std::uint64_t powerUnit : 4 = 0;
    std::uint64_t reserved : 60 = 0;

    static constexpr std::uint32_t MSR_PACKAGE_POWER_SKU_UNIT = 0x606;

    static PackagePowerSKUUnit Read() noexcept {
        return MSR::Read<PackagePowerSKUUnit>(MSR_PACKAGE_POWER_SKU_UNIT);
    }

    static void Write(const PackagePowerSKUUnit packagePowerSKUUnit) noexcept {
        MSR::Write(MSR_PACKAGE_POWER_SKU_UNIT, packagePowerSKUUnit);
    }
};
