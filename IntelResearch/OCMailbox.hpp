#pragma once

#include "Constants.hpp"
#include "MSR.hpp"
#include <cstdint>

namespace OCMailbox {
    constexpr std::uint32_t MISC_TURBO_RING_DOWNBIN_MASK = 0x00000002;
    constexpr std::uint32_t MISC_TURBO_RING_DOWNBIN_OFFSET = 1;;

    struct OCMailboxFull {
        std::uint32_t data = 0;
        std::uint8_t commandCompletion = 0;
        std::uint8_t param1 = 0;
        std::uint8_t param2 = 0;
        std::uint8_t reserved : 7 = 0;
        std::uint8_t runBusy : 1 = 0;
    };

    inline void SendCommand(const OCMailboxCommand cmd, const std::uint32_t data, const bool write) noexcept {
        OCMailboxFull full = OCMailboxFull();
        full.data = data;
        full.commandCompletion = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][write];
        full.param1 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2];
        full.param2 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][3];
        full.runBusy = 1;
        // clear runBusy?
        MSR::Write(MSRRegister::OCMAILBOX, reinterpret_cast<const std::uint64_t&>(full));
    }

    inline std::uint32_t Read(const OCMailboxCommand cmd, const std::uint32_t data = 0) noexcept {
        SendCommand(cmd, data, false);
        return static_cast<std::uint32_t>(MSR::Read(MSRRegister::OCMAILBOX));
    }

    inline void Write(const OCMailboxCommand cmd, const std::uint32_t data) noexcept {
        SendCommand(cmd, data, true);
    }
} // namespace OCMailbox
