#pragma once

#include <cstdint>

#ifdef _WIN32
#include "Windows/Driver.hpp"
#include <vector>
#else
#include <sys/mman.h>
#endif

namespace MMIO {
    inline std::uint32_t pageSize = 0;
    inline std::uint64_t pcieBase = 0;

    inline void InitPageSize() noexcept {
#ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        pageSize = sysInfo.dwPageSize;
#else
        pageSize = sysconf(_SC_PAGESIZE);
#endif
    }

    inline void InitPCIeBase() noexcept {
#ifdef _WIN32
        std::vector<std::uint8_t> table(GetSystemFirmwareTable('ACPI', 'GFCM', nullptr, 0));
        const DWORD tableSize = GetSystemFirmwareTable('ACPI', 'GFCM', table.data(), static_cast<const std::uint32_t>(table.size()));
        pcieBase = *reinterpret_cast<const std::uint64_t*>(&table[0x2C]);
#else
        const std::int32_t fd = open("/sys/firmware/acpi/tables/MCFG", O_RDONLY);
        lseek(fd, 0x2C, SEEK_SET);
        read(fd, reinterpret_cast<void*>(&pcieBase), sizeof(pcieBase));
        close(fd);
#endif
}

    inline std::uintptr_t MapAddress(const std::uintptr_t address) noexcept {
#ifdef _WIN32
        return Driver::MapAddress(address, pageSize);
#else
        const std::int32_t fd = open("/dev/mem", O_RDWR | O_SYNC);
        const std::uintptr_t mapped = reinterpret_cast<const std::uintptr_t>(mmap(nullptr, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (address & ~(pageSize - 1))));
        close(fd);
        return mapped + (address & (pageSize - 1));
#endif
    }

    inline void UnmapAddress(const std::uintptr_t mapped) noexcept {
#ifdef _WIN32
        Driver::UnmapAddress(mapped);
#else
        munmap(reinterpret_cast<void*>(mapped - (mapped & (pageSize - 1))), pageSize);
#endif
    }

    template<typename T>
    T Read(const std::uintptr_t address) noexcept {
#ifdef _WIN32
        return Driver::ReadMMIO<T>(address);
#else
        const std::uintptr_t mapped = MapAddress(address);
        const T value = *reinterpret_cast<const volatile T*>(mapped);
        UnmapAddress(mapped);
        return value;
#endif
    }

    template<typename T>
    void Write(const std::uintptr_t address, const T value) noexcept {
#ifdef _WIN32
        Driver::WriteMMIO<T>(address, value);
#else
        const std::uintptr_t mapped = MapAddress(address);
        *reinterpret_cast<const volatile T*>(mapped) = value;
        UnmapAddress(mapped);
#endif
    }

    inline std::uintptr_t MmPciBase(const std::uint32_t bus, const std::uint32_t device, const std::uint32_t function) noexcept {
        if (bus > 0xFF || device > 0x1F || function > 0x7)
            return 0;
        return pcieBase + (static_cast<std::uintptr_t>(bus) << 20) + (static_cast<std::uintptr_t>(device) << 15) + (static_cast<std::uintptr_t>(function) << 12);
    }

    inline std::uint32_t GetMchBar() noexcept {
        constexpr std::uintptr_t MCHBAR_OFFSET = 0x48;
        return MMIO::Read<std::uint32_t>(MmPciBase(0, 0, 0) + MCHBAR_OFFSET) & ~1;
    }
} // namespace MMIO
