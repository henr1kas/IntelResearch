#pragma once

#include <cstdint>

#ifdef _WIN32
#include "Windows/Driver.hpp"
#else
#include <sys/mman.h>
#endif

namespace MMIO {
    inline std::uint32_t pageSize = 0;

    inline void SetPageSize() noexcept {
#ifdef _WIN32
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);
        pageSize = sysInfo.dwPageSize;
#else
        pageSize = sysconf(_SC_PAGESIZE);
#endif
    }

    inline std::uintptr_t MapAddress(const std::uintptr_t address) noexcept {
#ifdef _WIN32
        return Driver::MapAddress(address, pageSize);
#else
        const std::int32_t fd = open("/dev/mem", O_RDWR | O_SYNC);
        // const type cast?
        const std::uint8_t* mapped = static_cast<std::uint8_t*>(mmap(nullptr, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (address & ~(pageSize - 1))));
        close(fd);
        return reinterpret_cast<const std::uintptr_t>(mapped + (address & (pageSize - 1)));
#endif
    }

    inline void UnmapAddress(const std::uintptr_t mapped) noexcept {
#ifdef _WIN32
        Driver::UnmapAddress(mapped);
#else
        // cast to void?, cast after arithmetic operation?, const type cast?
        munmap(reinterpret_cast<std::uint8_t*>(mapped) - (mapped & (pageSize - 1)), pageSize);
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
        constexpr std::uintptr_t PcdPciExpressBaseAddress = 0xE0000000; // TODO: get this from ACPI
        if (bus > 0xFF || device > 0x1F || function > 0x7)
            return 0;
        return PcdPciExpressBaseAddress + (static_cast<std::uintptr_t>(bus) << 20) + (static_cast<std::uintptr_t>(device) << 15) + (static_cast<std::uintptr_t>(function) << 12);
    }

    inline std::uint32_t GetMchBar() noexcept {
        constexpr std::uintptr_t MCHBAR_OFFSET = 0x48;
        return MMIO::Read<std::uint32_t>(MmPciBase(0, 0, 0) + MCHBAR_OFFSET) & ~1;
    }
} // namespace MMIO
