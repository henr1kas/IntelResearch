#pragma once

#include "../Impl/MMIO.hpp"
#include "SDK/Mailbox/Pcode/PcodeMailboxCC.hpp"
#include "SDK/Mailbox/Pcode/PcodeMailboxFull.hpp"
#include "SDK/MMIO/MchBar.hpp"

namespace PcodeMailbox {
    constexpr std::uintptr_t PCODE_MAILBOX_DATA_OFFSET = 0x5DA0;
    constexpr std::uintptr_t PCODE_MAILBOX_INTERFACE_OFFSET = 0x5DA4;

    inline PcodeMailboxFull SendCommand(const std::uint32_t command, const std::uint32_t data, const bool writeData) noexcept {
        const std::uint32_t mchBar = MchBar::Get();

        PcodeMailboxFull full;
        full.Interface.interfaceData = command;
        full.data = data;

        if (writeData)
            MMIO::Write(mchBar + PCODE_MAILBOX_DATA_OFFSET, full.data);

        full.Interface.Fields.runBusy = 1;
        MMIO::Write(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET, full.Interface.interfaceData);

        full.Interface.interfaceData = MMIO::Read<std::uint32_t>(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET);

        if (!writeData)
            full.data = MMIO::Read<std::uint32_t>(mchBar + PCODE_MAILBOX_DATA_OFFSET);

        return full;
    }

    inline PcodeMailboxCC Write(const std::uint32_t command, const std::uint32_t data) noexcept {
        const PcodeMailboxFull full = SendCommand(command, data, true);
        return static_cast<PcodeMailboxCC>(full.Interface.interfaceData);
    }

    inline PcodeMailboxCC Read(const std::uint32_t command, std::uint32_t& data) noexcept {
        const PcodeMailboxFull full = SendCommand(command, 0, false);
        data = full.data;
        return static_cast<PcodeMailboxCC>(full.Interface.Fields.command);
    }
} // namespace PcodeMailbox
