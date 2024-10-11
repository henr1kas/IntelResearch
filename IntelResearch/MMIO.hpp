#pragma once

#include <cstdint>

#ifndef _WIN32
#include <sys/mman.h>
#endif

namespace MMIO {
#ifndef _WIN32
    inline const std::uint32_t pageSize = sysconf(_SC_PAGESIZE);
#endif

    inline std::uintptr_t MapAddress(const std::uintptr_t address) noexcept {
#ifndef _WIN32
        const std::int32_t fd = open("/dev/mem", O_RDWR | O_SYNC);
        const std::uint8_t* mapped = static_cast<std::uint8_t*>(mmap(nullptr, pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (address & ~(pageSize - 1))));
        close(fd);
        return reinterpret_cast<const std::uintptr_t>(mapped + (address & (pageSize - 1)));
#endif
    }

    inline void UnmapAddress(const std::uintptr_t mapped) noexcept {
#ifndef _WIN32
        munmap(reinterpret_cast<std::uint8_t*>(mapped) - (mapped & (pageSize - 1)), pageSize);
#endif
    }

    template <typename T>
    T Read(const std::uintptr_t address) noexcept {
        const std::uintptr_t mapped = MapAddress(address);
        const T value = *reinterpret_cast<volatile T*>(mapped);
        UnmapAddress(mapped);
        return value;
    }

    template <typename T>
    void Write(const std::uintptr_t address, const T value) noexcept {
        const std::uintptr_t mapped = MapAddress(address);
        *reinterpret_cast<volatile T*>(mapped) = value;
        UnmapAddress(mapped);
    }

    inline std::uintptr_t MmPciBase(const std::uint32_t bus, const std::uint32_t device, const std::uint32_t function) noexcept {
        constexpr std::uintptr_t PcdPciExpressBaseAddress = 0xE0000000; // TODO: how to get this dynamic
        if (bus > 0xFF || device > 0x1F || function > 0x7)
            return 0;
        return PcdPciExpressBaseAddress + (bus << 20) + (device << 15) + (function << 12);
    }
    
    inline std::uint32_t GetMchBar() noexcept {
        constexpr std::uintptr_t MCHBAR_OFFSET = 0x48;
        return MMIO::Read<std::uint32_t>(MmPciBase(0, 0, 0) + MCHBAR_OFFSET) &~1;
    }
} // namespace MMIO
