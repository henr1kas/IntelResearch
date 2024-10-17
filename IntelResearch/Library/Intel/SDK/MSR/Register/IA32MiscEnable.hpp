#pragma once

#include "../../../../Impl/MSR.hpp"
#include <cstdint>

struct IA32MiscEnable : MSR::Register<IA32MiscEnable, 0x1A0> {
    std::uint64_t fse : 1;
    std::uint64_t reserved1 : 2;
    std::uint64_t tme : 1;
    std::uint64_t reserved2 : 12;
    std::uint64_t eist : 1;
    std::uint64_t reserved3 : 1;
    std::uint64_t monitor : 1;
    std::uint64_t reserved4 : 3;
    std::uint64_t cpuIDMax : 1;
    std::uint64_t tprDis : 1;
    std::uint64_t reserved5 : 10;
    std::uint64_t xd : 1;
    std::uint64_t reserved6 : 3;
    std::uint64_t turboDisable : 1;
    std::uint64_t reserved7 : 25;
};
