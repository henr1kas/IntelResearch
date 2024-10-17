#pragma once

#include <cstdint>

enum class OCMailboxCC : std::uint8_t {
    SUCCESS,
    OCLOCKED,
    INVALIDDOMAIN,
    MAXRATIOEXCEEDED,
    MAXVOLTAGEEXCEEDED,
    OCNOTSUPPORTED,
    WRITEFAILED,
    READFAILED
};
