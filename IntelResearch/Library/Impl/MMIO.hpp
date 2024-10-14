#pragma once

#ifdef _WIN32
#include "Windows/Driver.hpp"
#else
#include <sys/mman.h>
#endif

#include <cstdint>

namespace MMIO {
    inline std::uintptr_t MapAddress(const std::uintptr_t address) noexcept {
#ifdef _WIN32
        return Driver::MapAddress(address, Library::pageSize);
#else
        const std::int32_t fd = open("/dev/mem", O_RDWR | O_SYNC);
        const std::uintptr_t mapped = reinterpret_cast<const std::uintptr_t>(mmap(nullptr, Library::pageSize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (address & ~(Library::pageSize - 1))));
        close(fd);
        return mapped + (address & (Library::pageSize - 1));
#endif
    }

    inline void UnmapAddress(const std::uintptr_t mapped) noexcept {
#ifdef _WIN32
        Driver::UnmapAddress(mapped);
#else
        munmap(reinterpret_cast<void*>(mapped - (mapped & (Library::pageSize - 1))), Library::pageSize);
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
} // namespace MMIO
