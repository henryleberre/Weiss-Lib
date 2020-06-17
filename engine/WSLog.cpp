#include "WSLog.h"

namespace WS {

#ifdef __WEISS__OS_WINDOWS

    template <typename _T, typename ..._Args>
    void PrintError(const _T& message0, _Args... args) WS_NOEXCEPT {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
        WS::Print(message0, args...);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

    template <typename _T, typename ..._Args>
    void PrintSuccess(const _T& message0, _Args... args) WS_NOEXCEPT {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
        WS::Print(message0, args...);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    }

#elif defined(__WEISS__OS_LINUX)

    template <typename _T, typename ..._Args>
    void PrintError(const _T& message0, _Args... args) WS_NOEXCEPT
    {
        std::cout << "\x1B[1;31m";
        WS::Print(message0, args...);
        std::cout << "\x1B[1;37m";
    }

    template <typename _T, typename ..._Args>
    void PrintSuccess(const _T& message0, _Args... args) WS_NOEXCEPT
    {
        std::cout << "\x1B[1;32m";
        WS::Print(message0, args...);
        std::cout << "\x1B[1;37m";
    }

#else

	#error WSLog Isn't Supported On Your Platform

#endif

}; // WS