#include <iostream>
#include <thread>

#include "Constants.hpp"
#include "MSR.hpp"
#include "OCMailbox.hpp"
#include "Utils.hpp"

#ifdef _WIN32
#include "Driver.hpp"
#endif

int main() {
#ifdef _WIN32
    // TODO: load driver ourselves ?
    if (!Driver::OpenHandle()) {
        std::cerr << "ThrottleStop driver not found!\n";
        return 1;
    }
#endif

    /*
    Utils::SetCacheRatioOffset(2);
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

#ifdef _WIN32
    Driver::CloseHandle();
#endif
    return 0;
}
