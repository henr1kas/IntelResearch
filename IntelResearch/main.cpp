#include "Library/Library.hpp"

#include <iostream>
#include <thread>

int main() {
    Library::Init();

    /* Disable Ring Downbin */
    MiscTurboControl miscTurboControl = MiscTurboControl::Read().data;
    miscTurboControl.ringDownbin = 1;
    MiscTurboControl::Write(miscTurboControl);

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
    auto data = IA32MiscEnable::Read();
    data.turboDisable = 0; // not disabling rn
    IA32MiscEnable::Write(data);

    /* Turbo ratio limits */
    auto turboRatioLimit = TurboRatioLimit::Read();
    for (std::uint8_t i = 0; i < 8; ++i)
        turboRatioLimit.limit[i] = 33;
    TurboRatioLimit::Write(turboRatioLimit);

    /* Set Ring Ratio Limit */
    RingRatioLimit ringRatioLimit = RingRatioLimit::Read();
    ringRatioLimit.ringMaxOcRatio = 33;
    RingRatioLimit::Write(ringRatioLimit);

    /* Undervolt */
    for (std::uint8_t i = 0; i <= 4; ++i) {
        /* CPU core and cache handled per core below */
        if (i == 0 || i == 2)
            continue;
        VoltageFrequencySettings voltageFrequencySettings = VoltageFrequencySettings::Read(i).data;
        voltageFrequencySettings.SetConvertedVoltageOffset(-72);
        VoltageFrequencySettings::Write(voltageFrequencySettings, i);
    }

    /* Undervolt per ratio */
    VoltageFrequencySettings vfCore = VoltageFrequencySettings::Read(0).data;
    VoltageFrequencySettings vf = VoltageFrequencySettings::Read(2).data;
    while (true) {
        switch (UncoreFrequency::Read().uncoreFrequency) {
            case 30: {
                vf.SetConvertedVoltageOffset(-148); // -150 err 960M (-148 untetsd)
                break;
            }
            case 31: {
                vf.SetConvertedVoltageOffset(-152); // -154 err 960M
                break;
            }
            case 32: {
                vf.SetConvertedVoltageOffset(-156); // -158 err 960M
                break;
            }
            case 33: {
                vf.SetConvertedVoltageOffset(-158); // -160 err 960M
                break;
            }
                /* IDK HOW TO TEST THESE PROPERTLY */
            case 40: {
                vf.SetConvertedVoltageOffset(-150); // -152 err 180M
                break;
            }
            default: {
                vf.SetConvertedVoltageOffset(-72); // just a rand value
                break;
            }
        }

        vfCore.voltageOffsetS11 = vf.voltageOffsetS11;
        VoltageFrequencySettings::Write(vfCore, 0);
        VoltageFrequencySettings::Write(vf, 2);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    /* TODO: Speed Shift EPP, Limit reasons , imgui gui, config loading */

    Library::Deinit();
    return 0;
}
