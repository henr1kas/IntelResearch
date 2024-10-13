#pragma once

#include <cstdint>

#ifdef _WIN32
#include "../../Windows/Driver.hpp"
#else
#include <fcntl.h>
#include <string>
#include <vector>
#endif

namespace MSR {
    template<typename T>
    void Write(const std::uint32_t reg, const T val) noexcept {
        static_assert(sizeof(T) == 8, "MSR::Write: sizeof(T) must be 8 bytes");
#ifdef _WIN32
        Driver::WriteMSR(reg, val);
#else
        for (const std::string& str : Library::cpuStrCache) {
            const std::int32_t fd = open(str.data(), O_WRONLY);
            lseek(fd, reg, SEEK_SET);
            write(fd, &val, sizeof(val));
            close(fd);
        }
#endif
    }

    template<typename T = std::uint64_t>
    T Read(const std::uint32_t reg) noexcept {
        static_assert(sizeof(T) == 8, "MSR::Read: sizeof(T) must be 8 bytes");
#ifdef _WIN32
        return Driver::ReadMSR<T>(reg);
#else
        std::uint64_t val;
        int fd = open("/dev/cpu/0/msr", O_RDONLY);
        lseek(fd, reg, SEEK_SET);
        read(fd, &val, sizeof(val));
        close(fd);
        return val;
#endif
    }
} // namespace MSR
