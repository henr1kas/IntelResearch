#pragma once

#include "../../../../PcodeMailbox.hpp"
#include <cstdint>

struct VRTopologyData : PcodeMailbox::Command<VRTopologyData, 0x80000018, 0xFFFFFFFF> {
    std::uint32_t vrSaAddress : 4;
    std::uint32_t vrSaSvidType : 1;
    std::uint32_t vrIaAddress : 4;
    std::uint32_t vrIaSvidType : 1;
    std::uint32_t vrRingAddress : 4;
    std::uint32_t vrRingSvidType : 1;
    std::uint32_t vrGtusAddress : 4;
    std::uint32_t vrGtusSvidType : 1;
    std::uint32_t vrGtsAddress : 4;
    std::uint32_t vrGtsSvidType : 1;
    std::uint32_t reserved : 5;
    std::uint32_t svidDisable : 1;
    std::uint32_t psysImonDisable : 1;
};
