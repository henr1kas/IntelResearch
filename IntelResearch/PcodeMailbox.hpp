#pragma once

#include "MMIO.hpp"
#include "SDK/PcodeMailbox/PcodeMailboxFull.hpp"

namespace PcodeMailbox {
    constexpr std::uintptr_t PCODE_MAILBOX_DATA_OFFSET = 0x5DA0;
    constexpr std::uintptr_t PCODE_MAILBOX_INTERFACE_OFFSET = 0x5DA4;

    inline void Write(const std::uint32_t command, const std::uint32_t data) noexcept {
        PcodeMailboxFull full;
        full.Interface.interfaceData = command;
        full.data = data;

        const std::uint32_t mchBar = MMIO::GetMchBar();
        MMIO::Write(mchBar + PCODE_MAILBOX_DATA_OFFSET, full.data);

        full.Interface.Fields.runBusy = 1;
        MMIO::Write(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET, full.Interface.interfaceData);
    }

    inline PcodeMailboxFull Read(const std::uint32_t command) noexcept {
        PcodeMailboxFull full;
        full.Interface.interfaceData = command;
        full.Interface.Fields.runBusy = 1;

        const std::uint32_t mchBar = MMIO::GetMchBar();
        MMIO::Write(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET, full.Interface.interfaceData);

        full.Interface.interfaceData = MMIO::Read<std::uint32_t>(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET);
        full.data = MMIO::Read<std::uint32_t>(mchBar + PCODE_MAILBOX_DATA_OFFSET);

        return full;
    }
} // namespace PcodeMailbox
