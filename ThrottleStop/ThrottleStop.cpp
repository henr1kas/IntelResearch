#include <iostream>
#include "driver.hpp"

int main() {
    if (!driver::OpenHandle()) {
        std::cerr << "Throttlestop driver not found!\n";
        return 1;
    }

    const std::uint64_t val = driver::ReadMSR(0x1a2);

    std::cout << val << '\n';

    driver::CloseHandle();

    return 0;
}
