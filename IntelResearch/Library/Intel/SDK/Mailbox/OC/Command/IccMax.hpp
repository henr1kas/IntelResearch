#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

struct IccMax : OCMailbox::Command<IccMax, 0x16, 0x17> {
    std::uint32_t iccMax : 10 = 0;
    std::uint32_t reserved : 22 = 0;
};
