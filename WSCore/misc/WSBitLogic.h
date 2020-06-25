#pragma once

#include "../misc/WSPch.h"

namespace WS {

    // https://stackoverflow.com/questions/105252/how-do-i-convert-between-big-endian-and-little-endian-values-in-c
    template <typename _T>
    inline _T SwapEndian(const _T& u) noexcept
    {
        union
        {
            _T u;
            unsigned char u8[sizeof(_T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(_T); k++)
            dest.u8[k] = source.u8[sizeof(_T) - k - 1];

        return dest.u;
    }

    template <typename _T>
    inline _T ReverseBits(const _T& input) noexcept {
        _T output;

        for (size_t i = 0u; i < sizeof(_T) * 8u; i++)
            output |= ((input >> i) & 1) << (sizeof(_T) * 8u - i - 1u);

        return output;
    }

    inline bool IsCharUppercase(const char character) noexcept {
	    return character >= 0x41 && character <= 0x5A;
    }

    template <typename _T>
    inline _T FromBigEndian(const _T& val) WS_NOEXCEPT {
        if constexpr (std::endian::native == std::endian::big) {
            return val;
        } else if constexpr (std::endian::native == std::endian::little) {
            return WS::SwapEndian(val);
        } else {
            static_assert(std::is_same<_T, _T>::value, "Weiss Can't Handle Your Target Platform's Endianness");
        }
    }

    template <typename _T>
    inline _T FromLittleEndian(const _T& val) WS_NOEXCEPT {
        if constexpr (std::endian::native == std::endian::big) {
            return WS::SwapEndian(val);
        } else if constexpr (std::endian::native == std::endian::little) {
            return val;
        } else {
            static_assert(std::is_same<_T, _T>::value, "Weiss Can't Handle Your Target Platform's Endianness");
        }
    }

}; // WS