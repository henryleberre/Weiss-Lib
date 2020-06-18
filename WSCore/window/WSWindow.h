#pragma once

#include "../misc/WSPch.h"
#include "WSPeripheral.h"

#ifdef __WEISS__OS_LINUX

    #define __WEISS__XLIB_WINDOW_MASKS (ExposureMask)
    #define __WEISS__XLIB_ALL_MASKS (__WEISS__XLIB_WINDOW_MASKS | __WEISS__XLIB_MOUSE_MASKS | __WEISS__XLIB_KEYBOARD_MASKS)

#endif // __WEISS__OS_LINUX

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

        static constexpr const long windowMasks = ExposureMask;
        static constexpr const long mouseMasks = PointerMotionMask | ButtonPressMask | ButtonReleaseMask;
        static constexpr const long keyboardMasks = KeyPressMask | KeyReleaseMask;
        
        static constexpr const long masks = windowMasks | mouseMasks | keyboardMasks;

#endif

        Mouse    m_mouse;
        Keyboard m_keyboard;

    public:
        Window() = delete;

        Window(const char* title, const uint16_t width, const uint16_t height) WS_NOEXCEPT;

        void Show() WS_NOEXCEPT;
        void Hide() WS_NOEXCEPT;

        void Minimize() WS_NOEXCEPT;

        void Update() WS_NOEXCEPT;

        const Mouse&    GetMouse()    const noexcept { return this->m_mouse;    }
        const Keyboard& GetKeyboard() const noexcept { return this->m_keyboard; }

#ifdef __WEISS__OS_WINDOWS

        std::optional<LRESULT> WinHandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) WS_NOEXCEPT;

#endif // __WEISS__OS_WINDOWS

        ~Window() WS_NOEXCEPT;
    }; // Window Class

}; // WS