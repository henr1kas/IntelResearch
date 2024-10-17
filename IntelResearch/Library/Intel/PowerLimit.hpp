#pragma once

#include <cstdint>

struct PowerLimit {
    std::uint32_t powerLimit : 15 = 0;        // POWER_LIMIT_MASK
    std::uint32_t enable : 1 = 0;             // B_POWER_LIMIT_ENABLE
    std::uint32_t criticalPowerClamp : 1 = 0; // B_CRITICAL_POWER_CLAMP_ENABLE
    std::uint32_t time : 7 = 0;               // POWER_LIMIT_1_TIME_MASK
    std::uint32_t dutyCycle : 7 = 0;          // POWER_LIMIT_3_DUTY_CYCLE_MASK
    std::uint32_t lock : 1 = 0;               // B_POWER_LIMIT_LOCK
};
