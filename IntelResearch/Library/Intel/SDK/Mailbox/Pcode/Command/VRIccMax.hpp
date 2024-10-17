#pragma once

#include "../../../../PcodeMailbox.hpp"
#include <cstdint>

struct VRIccMax : PcodeMailbox::Command<VRIccMax, 0x80000518, 0x80000618> {
    std::uint32_t iccMax : 32;
};
