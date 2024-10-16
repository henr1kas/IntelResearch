#pragma once

#include "../Impl/MSR.hpp"
#include "SDK/DomainID.hpp"
#include "SDK/Mailbox/OC/OCMailboxFull.hpp"
#include <cstdint>

namespace OCMailbox {
    /* TODO: is reading status possible? */
    template<typename T>
    OCMailboxFull<T> SendCommand(OCMailboxFull<T> full) noexcept {
        static_assert(sizeof(T) == 4, "OCMailbox::SendCommand: sizeof(T) must be 4 bytes");
        constexpr std::uint32_t MSR_OC_MAILBOX = 0x150;
        full.Interface.Fields.runBusy = 1;
        MSR::Write(MSR_OC_MAILBOX, full);
        return MSR::Read<OCMailboxFull<T>>(MSR_OC_MAILBOX);
    }

    template<typename T, const std::uint8_t read, const std::uint8_t write>
    struct Command {
        static OCMailboxFull<T> Read(const std::uint8_t param1 = 0, const std::uint8_t param2 = 0) noexcept {
            OCMailboxFull<T> full;
            full.Interface.Fields.commandCompletion = read;
            full.Interface.Fields.param1 = param1;
            full.Interface.Fields.param2 = param2;
            return OCMailbox::SendCommand(full);
        }

        static OCMailboxFull<T> Write(const T data, const std::uint8_t param1 = 0, const std::uint8_t param2 = 0) noexcept {
            OCMailboxFull<T> full;
            full.data = data;
            full.Interface.Fields.commandCompletion = write;
            full.Interface.Fields.param1 = param1;
            full.Interface.Fields.param2 = param2;
            return OCMailbox::SendCommand(full);
        }
    };
} // namespace OCMailbox
