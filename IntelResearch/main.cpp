#include "Library/Library.hpp"

#include <iostream>

/*
inline std::uint8_t ReadTemperatureOffset() noexcept {
    return static_cast<std::uint8_t>((MSR::Read(MSRRegister::MSR1A2) & (127 << 24)) >> 24);
}
*/

int main() {
    Library::Init();

    /* Disable Ring Downbin */
    MiscTurboControl miscTurboControl = MiscTurboControl::Read().data;
    miscTurboControl.ringDownbin = 1;
    MiscTurboControl::Write(miscTurboControl);

    /* Set Ring Ratio Limit to 45 (TODO: does it do any difference) */
    RingRatioLimit ringRatioLimit = RingRatioLimit::Read();
    ringRatioLimit.ringMaxOcRatio = 45;
    RingRatioLimit::Write(ringRatioLimit);

    /* -70.3125mV on all domains */
    for (std::uint8_t i = 0; i <= 4; ++i) {
        VoltageFrequencySettings voltageFrequencySettings = VoltageFrequencySettings::Read(i).data;
        voltageFrequencySettings.SetConvertedVoltageOffset(-72);
        VoltageFrequencySettings::Write(voltageFrequencySettings, i);
    }

    /* Set IccMax for all VR domains to 255.75A in MSR/MMIO (MMIO auto update based on MSR val) */
    /* VR Domains i7-9750hf 0 - IA (core/cache), 1 - GT[s/us], 2 = SA */
    for (std::uint8_t i = 0; i <= 2; ++i) {
        auto curr = IccMax::Read(i).data;
        curr.iccMax = 1023;
        IccMax::Write(curr, i);
    }

    PackagePowerLimit packagePowerLimit = PackagePowerLimit::Read();
    PL3Control pl3Control = PL3Control::Read();
    PL4Control pl4Control = PL4Control::Read();

    auto& pl1 = packagePowerLimit.pl1;
    auto& pl2 = packagePowerLimit.pl2;
    auto& pl3 = pl3Control.pl3;
    auto& pl4 = pl4Control.pl4;

    pl1.powerLimit = 32767;
    pl1.criticalPowerClamp = 0;
    pl1.time = 127;
    pl2.powerLimit = 32767;
    pl2.time = 127;
    PackagePowerLimit::Write(packagePowerLimit);
    PL3Control::Write(pl3Control);
    PL4Control::Write(pl4Control);

    MMIO::Write(MchBar::Get() + 0x59A0ull, pl1);
    MMIO::Write(MchBar::Get() + 0x59A4ull, pl2);

    /* Turbo ratio limits are readonly on i7-9750hf */
    /* TODO: Speed Shift EPP */

    Library::Deinit();
    return 0;
}
