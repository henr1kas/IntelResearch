#pragma once

#include <cstdint>

namespace MchBar {
    inline std::uint32_t Get() noexcept {
        constexpr std::uintptr_t MCHBAR_OFFSET = 0x48;
        return MMIO::Read<std::uint32_t>(Library::MmPciBase(0, 0, 0) + MCHBAR_OFFSET) & ~1;
    }
} // namespace MchBar
