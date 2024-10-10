#pragma once

#include "Constants.hpp"
#include "MSR.hpp"
#include "SDK/DomainID.hpp"
#include "SDK/OCMailbox/OCMailboxFull.hpp"
#include "SDK/OCMailbox/VoltageFrequencySettings.hpp"
#include <cstdint>

namespace OCMailbox {
    inline OCMailboxFull SendCommand(const OCMailboxFull full) noexcept {
        MSR::Write(MSRRegister::OCMAILBOX, *reinterpret_cast<const std::uint64_t*>(&full));
        const std::uint64_t msrValue = MSR::Read(MSRRegister::OCMAILBOX);
        return *reinterpret_cast<const OCMailboxFull*>(&msrValue);
    }

    inline OCMailboxFull SendCommandWrapper(const OCMailboxCommand cmd, const std::uint32_t data, const bool write) noexcept {
        OCMailboxFull full = OCMailboxFull();
        full.data = data;
        full.commandCompletion = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][write];
        full.param1 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][2];
        full.param2 = ocMailboxCommands[static_cast<std::uint64_t>(cmd)][3];
        full.runBusy = 1;
        // clear runBusy?
        return SendCommand(full);
    }

    inline OCMailboxFull Read(const OCMailboxCommand cmd, const std::uint32_t data = 0) noexcept {
        return SendCommandWrapper(cmd, data, false);
    }

    inline OCMailboxFull Write(const OCMailboxCommand cmd, const std::uint32_t data) noexcept {
        return SendCommandWrapper(cmd, data, true);
    }

    inline VoltageFrequencySettings GetVfSettings(const DomainID domainId) noexcept {
        OCMailboxFull full = OCMailboxFull();
        full.commandCompletion = 0x10;
        full.param1 = static_cast<std::uint8_t>(domainId);
        full.runBusy = 1;
        const OCMailboxFull tmp = SendCommand(full);
        return *reinterpret_cast<const VoltageFrequencySettings*>(&tmp);
    }

    inline OCMailboxFull SetVfSettings(const DomainID domainId, const VoltageFrequencySettings vfSettings) noexcept {
        OCMailboxFull full = OCMailboxFull();
        full.data = *reinterpret_cast<const std::uint32_t*>(&vfSettings);
        full.commandCompletion = 0x11;
        full.param1 = static_cast<std::uint8_t>(domainId);
        full.runBusy = 1;
        return SendCommand(full);
    }

    /* move idk where */
    constexpr std::uint32_t MISC_TURBO_RING_DOWNBIN_MASK = 0x00000002;
    constexpr std::uint32_t MISC_TURBO_RING_DOWNBIN_OFFSET = 1;

    constexpr std::uint32_t AVX2_RATIO_OFFSET_MASK = 0x000003E0;
    constexpr std::uint32_t AVX2_RATIO_OFFSET_OFFSET = 5;
    constexpr std::uint32_t AVX3_RATIO_OFFSET_MASK = 0x00007C00;
    constexpr std::uint32_t AVX3_RATIO_OFFSET_OFFSET = 10;

    inline bool ReadRingDownBin() noexcept {
        const OCMailboxFull mailboxData = Read(OCMailboxCommand::MISCTURBOCONTROL);
        return (~mailboxData.data & MISC_TURBO_RING_DOWNBIN_MASK) >> MISC_TURBO_RING_DOWNBIN_OFFSET;
    }

    inline OCMailboxFull WriteRingDownBin(const bool enabled) noexcept {
        const std::uint32_t mailboxData = (~static_cast<std::uint32_t>(enabled) << MISC_TURBO_RING_DOWNBIN_OFFSET) & MISC_TURBO_RING_DOWNBIN_MASK;
        return Write(OCMailboxCommand::MISCTURBOCONTROL, mailboxData);
    }

    inline void ReadAvxRatioOffset(std::uint8_t& avx2Ratio, std::uint8_t& avx3Ratio) noexcept {
        const OCMailboxFull mailboxData = Read(OCMailboxCommand::AVXRATIOOFFSET);
        avx2Ratio = static_cast<std::uint8_t>((mailboxData.data & AVX2_RATIO_OFFSET_MASK) >> AVX2_RATIO_OFFSET_OFFSET);
        avx3Ratio = static_cast<std::uint8_t>((mailboxData.data & AVX3_RATIO_OFFSET_MASK) >> AVX3_RATIO_OFFSET_OFFSET);
    }

    inline void WriteAvxRatioOffset(const std::uint8_t avx2Ratio, const std::uint8_t avx3Ratio) noexcept {
        const std::uint32_t MailboxData = ((avx2Ratio << AVX2_RATIO_OFFSET_OFFSET) & AVX2_RATIO_OFFSET_MASK) |
                                          ((avx3Ratio << AVX3_RATIO_OFFSET_OFFSET) & AVX3_RATIO_OFFSET_MASK);
        Write(OCMailboxCommand::AVXRATIOOFFSET, MailboxData);
    }
} // namespace OCMailbox
