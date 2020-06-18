#pragma once

#include "../misc/WSPch.h"

namespace WS {

    template <typename _T, typename ..._Args>
    inline void Print(const _T& message0, _Args... args) WS_NOEXCEPT
    {
        std::cout << message0;
        (std::cout << ... << args);
        std::cout << '\n';
    }

    template <typename _T, typename ..._Args>
    void PrintError(const _T& message0, _Args... args) WS_NOEXCEPT;

    template <typename _T, typename ..._Args>
    void PrintSuccess(const _T& message0, _Args... args) WS_NOEXCEPT;

}; // WS