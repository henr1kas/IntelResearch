#pragma once

#include <cstdint>
#include "Constants.hpp"
#include "MSR.hpp"

namespace OCMailbox {
    inline std::uint64_t Read(const OCMailboxCommand cmd) noexcept {
        std::uint64_t value = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][0] << static_cast<std::uint64_t>(OCMailboxBits::CMD);
        value |= (1ull << static_cast<std::uint64_t>(OCMailboxBits::BUSYBIT));
        value |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2] << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        MSR::Write(MSRRegister::OCMAILBOX, value);
        return MSR::Read(MSRRegister::OCMAILBOX);
    }

    inline void Write(const OCMailboxCommand cmd, const std::uint64_t data) noexcept {
        std::uint64_t value = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][1] << static_cast<std::uint64_t>(OCMailboxBits::CMD);
        value |= data;
        value |= (1ull << static_cast<std::uint64_t>(OCMailboxBits::BUSYBIT));
        value |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2] << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        MSR::Write(MSRRegister::OCMAILBOX, value);
        value |= ~(1ull << static_cast<std::uint64_t>(OCMailboxBits::BUSYBIT));
        MSR::Write(MSRRegister::OCMAILBOX, value);
    }
} // namespace OCMailbox
