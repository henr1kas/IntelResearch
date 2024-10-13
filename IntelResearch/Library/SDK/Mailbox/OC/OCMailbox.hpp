#pragma once

#include "../../MSR/MSR.hpp"
#include "../../DomainID.hpp"
#include "OCMailboxFull.hpp"
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
} // namespace OCMailbox
