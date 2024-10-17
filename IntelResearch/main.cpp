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

    /* TODO: MSR/MMIO PL[1-3] */

    Library::Deinit();
    return 0;
}
