#pragma once

#include <cstdint>

enum class PcodeMailboxCC : std::uint8_t {
    SUCCESS,
    ILLEGALCMD,
    TIMEOUT,
    ILLEGALDATA,
    EDRAMNOTFUNCTIONAL,
    ILLEGALVRID,
    VRINTERFACELOCKED,
    VRERROR,
    EDRAMCURRENTLYUNAVAILABLE = 0xA
};
