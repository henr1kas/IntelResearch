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
    inline void Write(const std::uint32_t reg, const std::uint64_t val) noexcept {
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

    inline std::uint64_t Read(const std::uint32_t reg) noexcept {
#ifdef _WIN32
        return Driver::ReadMSR(reg);
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
