#pragma once

#include "../../../../../Impl/Intel/OCMailbox.hpp"
#include <cstdint>

struct MiscTurboControl {
    std::uint32_t reserved1 : 1 = 0;
    std::uint32_t ringDownbin : 1 = 0; // 0 = enabled, 1 = disabled
    std::uint32_t tvbRatio : 1 = 0;
    std::uint32_t tvbVoltage : 1 = 0;
    std::uint32_t reserved2 : 28 = 0;

    static OCMailboxFull<MiscTurboControl> Read() noexcept {
        OCMailboxFull<MiscTurboControl> full;
        full.Interface.Fields.commandCompletion = 0x18;
        return OCMailbox::SendCommand(full);
    }

    static OCMailboxFull<MiscTurboControl> Write(const MiscTurboControl miscTurboControl) noexcept {
        OCMailboxFull<MiscTurboControl> full;
        full.data = miscTurboControl;
        full.Interface.Fields.commandCompletion = 0x19;
        return OCMailbox::SendCommand(full);
    }
};
