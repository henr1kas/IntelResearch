#pragma once

#include "../../../../OCMailbox.hpp"
#include <cstdint>

struct OCCapabilities : OCMailbox::Command<OCCapabilities, 0x1, 0x0> {
    std::uint32_t maxOcRatioLimit : 8;
    std::uint32_t ratioOcSupported : 1;
    std::uint32_t voltageOverridesSupported : 1;
    std::uint32_t voltageOffsetSupported : 1;
    std::uint32_t domainId : 8;
    std::uint32_t reserved : 13;
};
