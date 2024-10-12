#pragma once

#ifdef _WIN32
#include <vector>
#include "Windows/Driver.hpp"
#endif

namespace Library {
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

    inline std::uint8_t Init() noexcept {
        InitPageSize();
        InitPCIeBase();
#ifdef _WIN32
        if (!Driver::OpenHandle()) {
            std::string driverPath(MAX_PATH, '\0');
            const DWORD dwRet = GetCurrentDirectoryA(MAX_PATH, driverPath.data());
            driverPath.resize(dwRet);
            driverPath += "\\Driver.sys";

            if (!dwRet)
                return 1;

            if (!Driver::LoadDriver("IntelResearch", driverPath))
                return 2;

            if (!Driver::OpenHandle())
                return 3;
        }
#endif
        return 0;
    }

    inline void Deinit() noexcept {
#ifdef _WIN32
        Driver::CloseHandle();
        Driver::UnloadDriver("IntelResearch");
#endif
    }

    inline std::uintptr_t MmPciBase(const std::uint32_t bus, const std::uint32_t device, const std::uint32_t function) noexcept {
        if (bus > 0xFF || device > 0x1F || function > 0x7)
            return 0;
        return pcieBase + (static_cast<std::uintptr_t>(bus) << 20) + (static_cast<std::uintptr_t>(device) << 15) + (static_cast<std::uintptr_t>(function) << 12);
    }
} // namespace Library

#include "SDK/DomainID.hpp"
#include "SDK/Mailbox/OC/OCMailbox.hpp"
#include "SDK/Mailbox/OC/OCMailboxFull.hpp"
#include "SDK/Mailbox/OC/VoltageFrequencySettings.hpp"
#include "SDK/MMIO/MMIO.hpp"
#include "SDK/MSR/MSR.hpp"
