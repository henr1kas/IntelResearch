#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct PowerCtl : MSR::Register<PowerCtl, 0x1FC> {
    std::uint64_t biProchot : 1;
    std::uint64_t c1E : 1;
    std::uint64_t reserved1 : 16;
    std::uint64_t energyEfficientPStateEnable : 1;
    std::uint64_t energyEfficientTurboDisable : 1;
    std::uint64_t raceToHaltDisable : 1; 
    std::uint64_t disableProchotOut : 1;
    std::uint64_t prochotResponse : 1;
    std::uint64_t prochotLock : 1;
    std::uint64_t disableVrThermalAlert : 1;
    std::uint64_t reserved2 : 5;
    std::uint64_t cstatePreWakeDisable : 1;
    std::uint64_t reserved3 : 33;
};
