#pragma once

#include "WSPch.h"

namespace WS {

#ifdef __WEISS__OS_WINDOWS

    static LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // __WEISS__OS_WINDOWS
    
    class Window {
    private:
#ifdef __WEISS__OS_WINDOWS

        HWND m_windowsNativeHandle = NULL;

#endif // __WEISS__OS_WINDOWS

    public:
        Window() = delete;

        Window(const char* title, const uint16_t width, const uint16_t height) WS_NOEXCEPT;

        void Update() WS_NOEXCEPT;

#ifdef __WEISS__OS_WINDOWS

        std::optional<LRESULT> WinHandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) WS_NOEXCEPT;

#endif // __WEISS__OS_WINDOWS

        ~Window() WS_NOEXCEPT;
    }; // Window Class

}; // WS