#pragma once

#include "../Impl/MMIO.hpp"
#include "SDK/MMIO/MchBar.hpp"
#include "SDK/Mailbox/Pcode/PcodeMailboxCC.hpp"
#include "SDK/Mailbox/Pcode/PcodeMailboxFull.hpp"

namespace PcodeMailbox {
    template<typename T>
    PcodeMailboxFull<T> SendCommand(const std::uint32_t command, const T data, const bool writeData) noexcept {
        constexpr std::uintptr_t PCODE_MAILBOX_DATA_OFFSET = 0x5DA0;
        constexpr std::uintptr_t PCODE_MAILBOX_INTERFACE_OFFSET = 0x5DA4;

        const std::uint32_t mchBar = MchBar::Get();

        PcodeMailboxFull<T> full;
        full.Interface.interfaceData = command;
        full.data = data;

        if (writeData)
            MMIO::Write(mchBar + PCODE_MAILBOX_DATA_OFFSET, full.data);

        full.Interface.Fields.runBusy = 1;
        MMIO::Write(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET, full.Interface.interfaceData);

        full.Interface.interfaceData = MMIO::Read<std::uint32_t>(mchBar + PCODE_MAILBOX_INTERFACE_OFFSET);

        if (!writeData)
            full.data = MMIO::Read<T>(mchBar + PCODE_MAILBOX_DATA_OFFSET);

        return full;
    }

    template<typename T = std::uint32_t>
    PcodeMailboxFull<T> Write(const std::uint32_t command, const T data) noexcept {
        return SendCommand(command, data, true);
    }

    template<typename T = std::uint32_t>
    PcodeMailboxFull<T> Read(const std::uint32_t command) noexcept {
        return SendCommand<T>(command, T(), false);
    }

    template<typename T, const std::uint32_t read, const std::uint32_t write>
    struct Command {
        static PcodeMailboxFull<T> Read(const std::uint32_t extra = 0) noexcept {
            return PcodeMailbox::Read<T>(read | extra);
        }

        static PcodeMailboxFull<T> Write(const T data, const std::uint32_t extra = 0) noexcept {
            return PcodeMailbox::Write(write | extra, data);
        }
    };
} // namespace PcodeMailbox
