#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

struct AvxRatioOffset : OCMailbox::Command<AvxRatioOffset, 0x1A, 0x1B> {
    std::uint32_t reserved1 : 5 = 0;
    std::uint32_t avx2Ratio : 5 = 0;
    std::uint32_t avx3Ratio : 5 = 0;
    std::uint32_t reserved2 : 17 = 0;
};
