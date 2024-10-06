#pragma once

#include <Windows.h>

/* Throttlestop.sys ioctl:
0x80006430 -> idk
0x80006434 -> idk
0x80006448 -> rdmsr
0x8000644C -> wrmsr
0x8000645C -> idk
0x80006460 -> idk
0x80006494 -> idk
0x80006498 -> idk
0x8000649C -> idk
0x800064A0 -> idk
0x800064A4 -> idk
*/

struct _rdmsr {
    std::uint32_t reg;
};

/* ThrottleStop driver takes val as 2 uint32_t's low/high. Pack to fix struct size. */
#pragma pack(push, 1)
struct _wrmsr {
    std::uint32_t reg;
    std::uint64_t val;
};
#pragma pack(pop)

namespace Driver {
    inline HANDLE handle = nullptr;

    inline bool OpenHandle() noexcept {
        handle = CreateFileA("\\\\.\\ThrottleStop", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
        return handle && handle != INVALID_HANDLE_VALUE;
    }

    inline void CloseHandle() noexcept {
        ::CloseHandle(handle);
    }

    inline std::uint64_t ReadMSR(const std::uint32_t reg) noexcept {
        std::uint64_t val = 0;
        _rdmsr arguments = {0};

        arguments.reg = reg;

        DeviceIoControl(handle, 0x80006448, &arguments, sizeof(arguments), &val, sizeof(val), nullptr, nullptr);

        return val;
    }

    inline void WriteMSR(const std::uint32_t reg, const std::uint64_t val) noexcept {
        _wrmsr arguments = {0};

        arguments.reg = reg;
        arguments.val = val;

        DeviceIoControl(handle, 0x8000644C, &arguments, sizeof(arguments), nullptr, 0, nullptr, nullptr);
    }
} // namespace driver
