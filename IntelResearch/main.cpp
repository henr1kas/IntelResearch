#include "Library/Library.hpp"

#include <iostream>
//#include <thread>

int main() {
    Library::Init();

    /*
    Utils::SetRingDownBin(false);
    std::uint64_t prev = 0, curr = 0;

    while (true) {
        curr = MSR::Read(MSRRegister::MSR621);
        //if (curr != prev)
        //    Utils::SetVoltageOffset(1, undervoltTbl[curr]);
        prev = curr;
        std::cout << curr << '\n';
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    */
    const std::uint32_t mchBar = MMIO::GetMchBar();
    const std::uint32_t pl2 = MMIO::Read<std::uint32_t>(mchBar + 0x59A0 + 0x4);
    std::cout << "Your MMIO PL2 is:" << (pl2 & 0x7FFF) / 8 << '\n';
    Library::Deinit();
    return 0;
}
