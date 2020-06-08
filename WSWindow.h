#pragma once

#include "WSPch.h"

namespace WS {

#ifdef __WEISS__OS_WINDOWS

    static LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif // __WEISS__OS_WINDOWS
    
    class Window {
    private:
#ifdef __WEISS__OS_WINDOWS

        HWND m_windowHandle = NULL;

#elif defined(__WEISS__OS_LINUX)

        ::Display* m_pDisplayHandle;
        ::Window   m_windowHandle;

#endif

    public:
        Window() = delete;

        Window(const char* title, const uint16_t width, const uint16_t height) WS_NOEXCEPT;

        void Show() WS_NOEXCEPT;
        void Hide() WS_NOEXCEPT;

        void Minimize() WS_NOEXCEPT;

        void Update() WS_NOEXCEPT;

#ifdef __WEISS__OS_WINDOWS

        std::optional<LRESULT> WinHandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) WS_NOEXCEPT;

#endif // __WEISS__OS_WINDOWS

        ~Window() WS_NOEXCEPT;
    }; // Window Class

}; // WS