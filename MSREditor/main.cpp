#include <iostream>
#include <thread>

#include "Constants.hpp"
#include "MSR.hpp"
#include "OCMailbox.hpp"
#include "Utils.hpp"

#ifdef _WIN32
#include "Driver.hpp"
#endif

/*
std::uint8_t ReadTemperatureOffset() noexcept {
    return static_cast<std::uint8_t>((MSR::Read(MSRRegister::UNK1A2) & (127 << 24)) >> 24);
}

std::uint64_t GetCacheRatioOffset() noexcept {
    return GetOCMailbox(OC_MAILBOX_RING_RATIO_OFFSET);
}

void SetCacheRatioOffset(const std::uint64_t ratio) noexcept {
    SetOCMailbox(OC_MAILBOX_RING_RATIO_OFFSET_SET, ratio);
}
*/

int main() {
#ifdef _WIN32
    // TODO: load driver ourselves ?
    if (!Driver::OpenHandle()) {
        std::cerr << "ThrottleStop driver not found!\n";
        return 1;
    }
#endif
    /*
    std::uint64_t prev = 0, curr = 0;

    while (true) {
        curr = MSR::Read(MSRRegister::UNCORECURRENTRATIO);
        if (curr != prev) {
            // undervolt
        }
        prev = curr;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    */
    std::cout << Utils::GetVoltageOffset(0) << '\n';

#ifdef _WIN32
    Driver::CloseHandle();
#endif
    return 0;
}
