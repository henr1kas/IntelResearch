#pragma once

#include "Constants.hpp"
#include "MSR.hpp"
#include <cstdint>

namespace OCMailbox {
    struct OCMailboxFull {
        std::uint32_t data = 0;
        std::uint8_t commandCompletion = 0;
        std::uint8_t param1 = 0;
        std::uint8_t param2 = 0;
        std::uint8_t reserved : 7 = 0;
        std::uint8_t runBusy : 1 = 1;
    };

    inline void SendCommand(const OCMailboxCommand cmd, const std::uint32_t data, const bool write) noexcept {
        OCMailboxFull full = OCMailboxFull();
        full.data = data;
        full.commandCompletion = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][write];
        full.param1 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2];
        full.param2 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][3];
        // clear runBusy?
        MSR::Write(MSRRegister::OCMAILBOX, reinterpret_cast<const std::uint64_t&>(full));
    }

    inline std::uint64_t Read(const OCMailboxCommand cmd, const std::uint32_t data = 0) noexcept {
        SendCommand(cmd, data, false);
        return MSR::Read(MSRRegister::OCMAILBOX);
    }

    inline void Write(const OCMailboxCommand cmd, const std::uint32_t data) noexcept {
        SendCommand(cmd, data, true);
    }
} // namespace OCMailbox
