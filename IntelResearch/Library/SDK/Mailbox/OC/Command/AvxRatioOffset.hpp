#pragma once

#include "../OCMailbox.hpp"
#include "../OCMailboxFull.hpp"
#include <cstdint>

struct AvxRatioOffset {
    std::uint32_t reserved1 : 5 = 0;
    std::uint32_t avx2Ratio : 5 = 0;
    std::uint32_t avx3Ratio : 5 = 0;
    std::uint32_t reserved2 : 17 = 0;

    static OCMailboxFull<AvxRatioOffset> Read() noexcept {
        OCMailboxFull<AvxRatioOffset> full;
        full.Interface.Fields.commandCompletion = 0x1A;
        return OCMailbox::SendCommand(full);
    }

    static OCMailboxFull<AvxRatioOffset> Write(const AvxRatioOffset avxRatioOffset) noexcept {
        OCMailboxFull<AvxRatioOffset> full;
        full.data = avxRatioOffset;
        full.Interface.Fields.commandCompletion = 0x1B;
        return OCMailbox::SendCommand(full);
    }
};
