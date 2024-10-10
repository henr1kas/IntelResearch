#pragma once

#include <cstdint>

#include "Constants.hpp"
#include "OCMailbox.hpp"
#include "SDK/OCMailbox/VoltageFrequencySettings.hpp"

namespace Utils {
    inline VoltageFrequencySettings GetVfSettings(const std::uint8_t domainId) noexcept {
        const std::uint32_t mailboxData = OCMailbox::Read(static_cast<OCMailboxCommand>(static_cast<std::uint8_t>(OCMailboxCommand::VOLTAGEFREQUENCY0) + domainId));
        return *reinterpret_cast<const VoltageFrequencySettings*>(&mailboxData);
    }

    inline void SetVfSettings(const std::uint8_t domainId, const VoltageFrequencySettings& vfSettings) noexcept {
        OCMailbox::Write(static_cast<OCMailboxCommand>(static_cast<std::uint8_t>(OCMailboxCommand::VOLTAGEFREQUENCY0) + domainId), *reinterpret_cast<const std::uint32_t*>(&vfSettings));
    }

    /*
    inline std::uint8_t ReadTemperatureOffset() noexcept {
        return static_cast<std::uint8_t>((MSR::Read(MSRRegister::MSR1A2) & (127 << 24)) >> 24);
    }
    */

    inline bool GetRingDownBin() noexcept {
        const std::uint64_t mailboxData = OCMailbox::Read(OCMailboxCommand::MISCTURBOCONTROL);
        return (~mailboxData & OCMailbox::MISC_TURBO_RING_DOWNBIN_MASK) >> OCMailbox::MISC_TURBO_RING_DOWNBIN_OFFSET;
    }

    inline void SetRingDownBin(const bool enabled) noexcept {
        const std::uint32_t mailboxData = (~static_cast<std::uint32_t>(enabled) << OCMailbox::MISC_TURBO_RING_DOWNBIN_OFFSET) & OCMailbox::MISC_TURBO_RING_DOWNBIN_MASK;
        OCMailbox::Write(OCMailboxCommand::MISCTURBOCONTROL, mailboxData);
    }
} // namespace Utils
