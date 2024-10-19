#include "Library/Library.hpp"

#include <iostream>
#include <thread>

int main() {
    Library::Init();

    /* Disable Ring Downbin */
    MiscTurboControl miscTurboControl = MiscTurboControl::Read().data;
    miscTurboControl.ringDownbin = 1;
    MiscTurboControl::Write(miscTurboControl);

    /* Set Ring Ratio Limit to 33 (TODO: does it do any difference) */
    RingRatioLimit ringRatioLimit = RingRatioLimit::Read();
    ringRatioLimit.ringMaxOcRatio = 33;
    RingRatioLimit::Write(ringRatioLimit);

    /* undervolt */
    for (std::uint8_t i = 0; i <= 4; ++i) {
        VoltageFrequencySettings voltageFrequencySettings = VoltageFrequencySettings::Read(i).data;
        voltageFrequencySettings.SetConvertedVoltageOffset(-72);
        if (i == 0)
            voltageFrequencySettings.SetConvertedVoltageOffset(-103);
        VoltageFrequencySettings::Write(voltageFrequencySettings, i);
    }

    /* Set IccMax for all VR domains to 255.75A in MSR/MMIO (MMIO auto update based on MSR val) */
    IccMax iccMax;
    iccMax.iccMax = 1023;
    /* VR Domains i7-9750hf 0 - IA (core/cache), 1 - GT[s/us], 2 = SA */
    for (std::uint8_t i = 0; i <= 2; ++i)
        IccMax::Write(iccMax, i);

    /* Set Power Limits */
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

    /* Disable turbo */
    /*
    auto data = IA32MiscEnable::Read();
    data.turboDisable = 1;
    IA32MiscEnable::Write(data);
    */

    /* Turbo ratio limits */
    auto turboRatioLimit = TurboRatioLimit::Read();
    for (std::uint8_t i = 0; i < 8; ++i)
        turboRatioLimit.limit[i] = 33;
    TurboRatioLimit::Write(turboRatioLimit);

    VoltageFrequencySettings voltageFrequencySettings = VoltageFrequencySettings::Read(3).data;
    while (true) {
        const auto freq = UncoreFrequency::Read().uncoreFrequency;
        if (freq <= 26) {
            voltageFrequencySettings.SetConvertedVoltageOffset(-72);
        } else {
            voltageFrequencySettings.SetConvertedVoltageOffset(-113);
        }

        VoltageFrequencySettings::Write(voltageFrequencySettings, 3);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    /* TODO: Speed Shift EPP, Limit reasons , imgui gui, config loading */

    Library::Deinit();
    return 0;
}
