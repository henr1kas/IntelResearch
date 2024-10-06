#pragma once

#include <cstdint>
#include "Constants.hpp"
#include "MSR.hpp"

namespace OCMailbox {
    inline void SendCommand(const OCMailboxCommand cmd, const std::uint32_t data = 0) noexcept {
        std::uint64_t value = data;
        value |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][data != 0] << static_cast<std::uint64_t>(OCMailboxBits::CMD));
        value |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2] << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
        value |= (ocMailboxCommands[static_cast<std::uint64_t>(cmd)][3] << static_cast<std::uint64_t>(OCMailboxBits::PARAM2));
        value |= (1ull << static_cast<std::uint64_t>(OCMailboxBits::RUNBUSY));

        // MSR::Write(MSRRegister::OCMAILBOX, value);
        // value |= ~(1ull << static_cast<std::uint64_t>(OCMailboxBits::RUNBUSY));
        MSR::Write(MSRRegister::OCMAILBOX, value);
    }

    inline std::uint64_t Read(const OCMailboxCommand cmd) noexcept {
        SendCommand(cmd);
        return MSR::Read(MSRRegister::OCMAILBOX);
    }

    inline void Write(const OCMailboxCommand cmd, const std::uint32_t data) noexcept {
        SendCommand(cmd, data);
    }
} // namespace OCMailbox
