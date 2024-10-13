#pragma once

#include "../MSR.hpp"

#include <cstdint>

struct PackagePowerSKU {
    std::uint64_t packageTdp : 15 = 0;
    std::uint64_t reserved1 : 17 = 0;
    std::uint64_t packageMaxPower : 15 = 0;
    std::uint64_t reserved2 : 17 = 0;

    static constexpr std::uint32_t MSR_PACKAGE_POWER_SKU = 0x614;

    static PackagePowerSKU Read() noexcept {
        const std::uint64_t packagePowerSKU = MSR::Read(MSR_PACKAGE_POWER_SKU);
        return *reinterpret_cast<const PackagePowerSKU*>(&packagePowerSKU);
    }

    static void Write(const PackagePowerSKU packagePowerSKU) noexcept {
        MSR::Write(MSR_PACKAGE_POWER_SKU, *reinterpret_cast<const std::uint64_t*>(&packagePowerSKU));
    }
};
