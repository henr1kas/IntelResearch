#pragma once

#include "Constants.hpp"
#include "MSR.hpp"
#include <cstdint>

namespace OCMailbox {
    inline void Write(const OCMailboxCommand cmd, std::uint64_t data) noexcept {
        data |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][data != 0] << static_cast<std::uint64_t>(OCMailboxBits::CMD));
        data |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2] << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        data |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][3] << static_cast<std::uint64_t>(OCMailboxBits::PARAM2));
        data |= (1ull << static_cast<std::uint64_t>(OCMailboxBits::RUNBUSY));

        // MSR::Write(MSRRegister::OCMAILBOX, data);
        // data |= ~(1ull << static_cast<std::uint64_t>(OCMailboxBits::RUNBUSY));
        MSR::Write(MSRRegister::OCMAILBOX, data);
    }

    inline std::uint64_t Read(const OCMailboxCommand cmd) noexcept {
        Write(cmd, 0);
        return MSR::Read(MSRRegister::OCMAILBOX);
    }
} // namespace OCMailbox
