#include <iostream>

#include "Constants.hpp"
#include "MSR.hpp"
#include "OCMailbox.hpp"

#ifdef _WIN32
#include "Driver.hpp"
#endif

/*
std::uint8_t ReadTemperatureOffset() noexcept {
    return static_cast<std::uint8_t>((Driver::ReadMSR(0x1a2) & (127 << 24)) >> 24);
}

double UnconvertRoundedOffset(const std::uint64_t y) noexcept {
    const std::int64_t x = y >> 21;
    return x <= 1024 ? static_cast<double>(x) : static_cast<double>(-(2048 - x));
}

double UnconvertOffset(const std::uint64_t y) noexcept {
    return UnconvertRoundedOffset(y) / 1.024;
}

double UnpackOffset(const std::uint64_t data) noexcept {
    const std::uint64_t plane = data / (1ull << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
    const std::uint64_t offset = data ^ (plane << static_cast<std::uint64_t>(OCMailboxBits::PARAM1));
    return UnconvertOffset(offset);
}

std::uint64_t PackOffset(const std::uint64_t plane, const std::uint64_t offset = 0) noexcept {
    return (1ull << static_cast<std::uint64_t>(OCMailboxBits::BUSYBIT)) |
           (plane << static_cast<std::uint64_t>(OCMailboxBits::PARAM1)) |
           (1ull << 36) |
           (static_cast<std::uint64_t>(offset != 0) << static_cast<std::uint64_t>(OCMailboxBits::CMD)) |
           offset;
}

double GetVoltageOffset(const std::uint8_t plane) noexcept {
    Driver::WriteMSR(0x150, PackOffset(plane));
    return UnpackOffset(Driver::ReadMSR(0x150));
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



#ifdef _WIN32
    Driver::CloseHandle();
#endif
    return 0;
}
