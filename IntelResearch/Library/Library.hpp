#pragma once

#ifdef _WIN32
#include "Impl/Windows/Driver.hpp"
#else
#include <fcntl.h>
#include <unistd.h>
#include <string>
#endif

#include <cstdint>
#include <vector>

namespace Library {
    inline std::uint32_t pageSize = 0;
    inline std::uint64_t pcieBase = 0;
#ifndef _WIN32
    std::vector<std::string> cpuStrCache;
#endif

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
#else
        cpuStrCache.resize(sysconf(_SC_NPROCESSORS_ONLN));
        for (std::size_t i = 0; i < cpuStrCache.size(); ++i) {
            cpuStrCache[i] = "/dev/cpu/" + std::to_string(i) + "/msr";
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

#include "Impl/MMIO.hpp"
#include "Impl/MSR.hpp"
#include "Intel/OCMailbox.hpp"
#include "Intel/PcodeMailbox.hpp"
#include "Intel/PowerLimit.hpp"
#include "Intel/SDK/Mailbox/OC/Command/AvxRatioOffset.hpp"
#include "Intel/SDK/Mailbox/OC/Command/IccMax.hpp"
#include "Intel/SDK/Mailbox/OC/Command/MiscTurboControl.hpp"
#include "Intel/SDK/Mailbox/OC/Command/OCCapabilities.hpp"
#include "Intel/SDK/Mailbox/OC/Command/PerCoreRatioLimit.hpp"
#include "Intel/SDK/Mailbox/OC/Command/VoltageFrequencySettings.hpp"
#include "Intel/SDK/Mailbox/OC/Command/PerCoreRatioLimit.hpp"
#include "Intel/SDK/Mailbox/OC/DomainID.hpp"
#include "Intel/SDK/Mailbox/OC/OCMailboxCC.hpp"
#include "Intel/SDK/Mailbox/OC/OCMailboxFull.hpp"
#include "Intel/SDK/Mailbox/OC/OCMailboxInterface.hpp"
#include "Intel/SDK/Mailbox/Pcode/Command/VRIccMax.hpp"
#include "Intel/SDK/Mailbox/Pcode/Command/VRTopologyData.hpp"
#include "Intel/SDK/Mailbox/Pcode/PcodeMailboxCC.hpp"
#include "Intel/SDK/Mailbox/Pcode/PcodeMailboxFull.hpp"
#include "Intel/SDK/Mailbox/Pcode/PcodeMailboxInterface.hpp"
#include "Intel/SDK/Mailbox/Pcode/SKLVRDomain.hpp"
#include "Intel/SDK/MMIO/MchBar.hpp"
#include "Intel/SDK/MSR/Register/IA32MiscEnable.hpp"
#include "Intel/SDK/MSR/Register/IA32ThermStatus.hpp"
#include "Intel/SDK/MSR/Register/PackagePowerLimit.hpp"
#include "Intel/SDK/MSR/Register/PackagePowerSKU.hpp"
#include "Intel/SDK/MSR/Register/PackagePowerSKUUnit.hpp"
#include "Intel/SDK/MSR/Register/PL3Control.hpp"
#include "Intel/SDK/MSR/Register/PL4Control.hpp"
#include "Intel/SDK/MSR/Register/PowerCtl.hpp"
#include "Intel/SDK/MSR/Register/RingRatioLimit.hpp"
#include "Intel/SDK/MSR/Register/TemperatureTarget.hpp"
#include "Intel/SDK/MSR/Register/TurboRatioLimit.hpp"
#include "Intel/SDK/MSR/Register/UncoreFrequency.hpp"
