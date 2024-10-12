#pragma once

#define NOMINMAX
#include <Windows.h>
#include <string>

/* TODO: use std::uint8_t[size] buffers in funcs instead of structs */
#pragma pack(push, 1)
template<typename T>
struct _WritePort {
    std::uint32_t port; // idk why not u16
    T data;
};
struct _ReadMSR {
    std::uint32_t reg;
};
struct _WriteMSR {
    std::uint32_t reg;
    std::uint64_t val;
};
struct _MapMemory {
    std::uint64_t address;
    std::uint32_t size;
};
template<typename T>
struct _WriteMMIO {
    std::uintptr_t address;
    T val;
};
struct _HalGetBusDataByOffset {
    /*
    std::uint8_t bus;
    std::uint8_t device : 5; 
    std::uint8_t function : 3;
    */
    std::uint32_t address;
    std::uint32_t offset;
};

struct _HalSetBusDataByOffset {
    /*
    std::uint8_t bus;
    std::uint8_t device : 5;
    std::uint8_t function : 3;
    */
    std::uint32_t address;
    std::uint32_t offset;
    std::uint32_t data;
};
#pragma pack(pop)

namespace Driver {
    inline HANDLE handle = nullptr;

    /* TODO: err handling */
    inline bool LoadDriver(const std::string& driverName, const std::string& driverPath) noexcept {
        const auto hSCManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
        if (!hSCManager)
            return false;

        auto hService = CreateServiceA(hSCManager, driverName.data(), driverName.data(), SERVICE_START | DELETE | SERVICE_STOP, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE,
                                       driverPath.data(),
                                       nullptr, nullptr, nullptr, nullptr, nullptr);

        if (!hService) {
            if (GetLastError() == ERROR_SERVICE_EXISTS)
                hService = OpenServiceA(hSCManager, driverName.data(), SERVICE_START | DELETE | SERVICE_STOP);
            else {
                CloseServiceHandle(hSCManager);
                return false;
            }
        }

        if (!StartServiceA(hService, 0, nullptr)) {
            DeleteService(hService);
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            return false;
        }

        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return true;
    }

    /* TODO: err handling */
    inline bool UnloadDriver(const std::string& driverName) noexcept {
        const auto hSCManager = OpenSCManagerA(nullptr, nullptr, SC_MANAGER_CREATE_SERVICE);
        if (!hSCManager)
            return false;

        const auto hService = OpenServiceA(hSCManager, driverName.data(), SERVICE_STOP | DELETE);
        if (!hService) {
            CloseServiceHandle(hSCManager);
            return false;
        }

        SERVICE_STATUS status;
        ControlService(hService, SERVICE_CONTROL_STOP, &status);

        if (!DeleteService(hService)) {
            CloseServiceHandle(hService);
            CloseServiceHandle(hSCManager);
            return false;
        }

        CloseServiceHandle(hService);
        CloseServiceHandle(hSCManager);
        return true;
    }

    inline bool OpenHandle() noexcept {
        handle = CreateFileA("\\\\.\\IntelResearch", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        return handle && handle != INVALID_HANDLE_VALUE;
    }

    inline void CloseHandle() noexcept {
        ::CloseHandle(handle);
    }

    /* untested */
    template<typename T>
    T ReadPort(const std::uint16_t port) noexcept {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4, "Driver::ReadPort: sizeof(T) must be 1/2/4 bytes");
        T val{};
        DeviceIoControl(handle, 0x80006430, &port, sizeof(port), &val, sizeof(val), nullptr, nullptr);
        return val;
    }

    /* untested */
    template<typename T>
    void WritePort(const std::uint16_t port, const T data) noexcept {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4, "Driver::WritePort: sizeof(T) must be 1/2/4 bytes");
        _WritePort<T> arguments = {0};
        arguments.port = port;
        arguments.data = data;
        DeviceIoControl(handle, 0x80006434, &arguments, sizeof(arguments), nullptr, nullptr, nullptr, nullptr);
    }

    inline std::uint64_t ReadMSR(const std::uint32_t reg) noexcept {
        std::uint64_t val = 0;
        _ReadMSR arguments = {0};
        arguments.reg = reg;
        DeviceIoControl(handle, 0x80006448, &arguments, sizeof(arguments), &val, sizeof(val), nullptr, nullptr);
        return val;
    }

    inline void WriteMSR(const std::uint32_t reg, const std::uint64_t val) noexcept {
        _WriteMSR arguments = {0};
        arguments.reg = reg;
        arguments.val = val;
        DeviceIoControl(handle, 0x8000644C, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    /* untested */
    inline std::uintptr_t MapAddress(const std::uintptr_t address, const std::uint32_t size) noexcept {
        _MapMemory arguments = {0};
        arguments.address = address;
        arguments.size = size;
        std::uintptr_t val = 0;
        DeviceIoControl(handle, 0x8000645C, &arguments, sizeof(arguments), &val, sizeof(val), nullptr, nullptr);
        return val;
    }

    /* untested */
    inline void UnmapAddress(std::uintptr_t address) noexcept {
        DeviceIoControl(handle, 0x80006460, &address, sizeof(address), nullptr, 0, nullptr, nullptr);
    }

    /* returns 1? maybe some counter. related to MapMemory */
    inline std::uint32_t unk6494() noexcept {
        std::uint32_t val = 0;
        DeviceIoControl(handle, 0x80006494, nullptr, 0, &val, sizeof(val), nullptr, nullptr);
        return val;
    }

    /* untested */
    template<typename T>
    T ReadMMIO(std::uintptr_t address) noexcept {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8, "Driver::ReadMMIO: sizeof(T) must be 1/2/4/8 bytes");
        T val{};
        DeviceIoControl(handle, 0x80006498, &address, sizeof(address), &val, sizeof(val), nullptr, nullptr);
        return val;
    }

    template<typename T>
    void WriteMMIO(const std::uintptr_t address, const T val) noexcept {
        static_assert(sizeof(T) == 1 || sizeof(T) == 2 || sizeof(T) == 4 || sizeof(T) == 8, "Driver::WriteMMIO: sizeof(T) must be 1/2/4/8 bytes");
        _WriteMMIO<T> arguments = {0};
        arguments.address = address;
        arguments.val = val;
        DeviceIoControl(handle, 0x8000649C, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }

    /* untested */
    inline _HalGetBusDataByOffset HalGetBusDataByOffset(const std::uint32_t address, const std::uint32_t offset) noexcept {
        _HalGetBusDataByOffset arguments = {0}, out = {0};
        arguments.address = address;
        arguments.offset = offset;
        DeviceIoControl(handle, 0x800064A0, &arguments, sizeof(arguments), &out, sizeof(out), nullptr, nullptr);
        return out;
    }

    /* untested */
    inline void HalSetBusDataByOffset(const std::uint32_t address, const std::uint32_t offset, const std::uint32_t data) noexcept {
        _HalSetBusDataByOffset arguments = {0};
        arguments.address = address;
        arguments.offset = offset;
        arguments.data = data;
        DeviceIoControl(handle, 0x800064A4, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }
} // namespace Driver
