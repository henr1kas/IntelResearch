#include <iostream>
#include <thread>

#include "Constants.hpp"
#include "MSR.hpp"
#include "OCMailbox.hpp"

#ifdef _WIN32
#include "Windows/Driver.hpp"
#endif

int main() {
#ifdef _WIN32
    if (!Driver::OpenHandle()) {
        std::string driverPath(MAX_PATH, '\0');
        const DWORD dwRet = GetCurrentDirectoryA(MAX_PATH, driverPath.data());
        driverPath.resize(dwRet);
        driverPath += "\\ThrottleStop.sys";

        if (!dwRet) {
            std::cerr << "Failed to get current directory. Error: " << GetLastError() << '\n';
            return 1;
        }

        if (!Driver::LoadDriver("IntelResearch", driverPath)) {
            std::cerr << "Failed to load ThrottleStop driver!\n";
            return 1;
        }
        if (!Driver::OpenHandle()) {
            std::cerr << "ThrottleStop driver not found!\n";
            return 1;
        }
    }
#endif

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
#ifdef _WIN32
    Driver::CloseHandle();
    Driver::UnloadDriver("IntelResearch");
#endif
    return 0;
}
