#pragma once

#include <cstdint>
#include "../../../DomainID.hpp"
#include "../OCMailboxFull.hpp"
#include "../../../../../Impl/Intel/OCMailbox.hpp"

struct VoltageFrequencySettings {
    std::uint32_t maxOcRatio : 8 = 0;
    std::uint32_t voltageTargetU12 : 12 = 0;
    std::uint32_t targetMode : 1 = 0;
    std::uint32_t voltageOffsetS11 : 11 = 0;

    constexpr std::int16_t GetConvertedVoltageOffset() const noexcept {
        return static_cast<std::int16_t>(voltageOffsetS11) - ((voltageOffsetS11 & 1024) << 1);
    }

    constexpr void SetConvertedVoltageOffset(const std::int16_t input) noexcept {
        voltageOffsetS11 = static_cast<std::uint16_t>(input + (input < 0) * (1 << 11));
    }

    constexpr static double GetAccurateVoltage(const std::int16_t input) noexcept {
        return 0.9765625 * input;
    }

    static OCMailboxFull<VoltageFrequencySettings> Read(const DomainID domainID) noexcept {
        /* TODO: test if 0x6 readable */
        if (domainID > DomainID::SYSTEMAGENT)
            return {};
        OCMailboxFull<VoltageFrequencySettings> full;
        full.Interface.Fields.commandCompletion = 0x10;
        full.Interface.Fields.param1 = static_cast<const std::uint8_t>(domainID);
        return OCMailbox::SendCommand(full);
    }

    static OCMailboxFull<VoltageFrequencySettings> Write(const DomainID domainID, const VoltageFrequencySettings vfSettings) noexcept {
        /* TODO: test if 0x6 readable */
        if (domainID > DomainID::SYSTEMAGENT)
            return {};
        OCMailboxFull<VoltageFrequencySettings> full;
        full.data = vfSettings;
        full.Interface.Fields.commandCompletion = 0x11;
        full.Interface.Fields.param1 = static_cast<std::uint8_t>(domainID);
        return OCMailbox::SendCommand(full);
    }
};
