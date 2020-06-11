#pragma once

#include "WSPch.h"

namespace WS {

    // https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
    template <typename T>
    inline T SwapEndian(const T& u) noexcept
    {
        union
        {
            T u;
            unsigned char u8[sizeof(T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];

        return dest.u;
    }

    template <typename T>
    inline T ReverseBits(const T& input) noexcept {
        T output;

        for (size_t i = 0u; i < sizeof(T) * 8u; i++)
            output |= ((input >> i) & 1) << (sizeof(T) * 8u - i - 1u);

        return output;
    }

    inline bool IsCharUppercase(const char character) noexcept {
	    return character >= 0x41 && character <= 0x5A;
    }

}; // WS