#include "WSWindow.h"

namespace WS {

#ifdef __WEISS__OS_WINDOWS

	static LRESULT CALLBACK WindowsWindowWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
#ifdef __WEISS__PLATFORM_X64
		Window* pWindow = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
#else
		Window* pWindow = reinterpret_cast<Window*>(GetWindowLong(hwnd, GWLP_USERDATA));
#endif

		if (pWindow != nullptr)
			return pWindow->WinHandleMessage(msg, wParam, lParam).value_or(DefWindowProc(hwnd, msg, wParam, lParam));

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	Window::Window(const char* title, const uint16_t width, const uint16_t height) WS_NOEXCEPT
	{
		const HINSTANCE hInstance = GetModuleHandle(NULL);

		// Step 1 : Register Window Class
		WNDCLASS wc = { };
		{
			wc.lpfnWndProc = WS::WindowsWindowWindowProc;
			wc.hInstance = hInstance;
			wc.lpszClassName = L"Weiss's Windows Window Class";

			RegisterClass(&wc);
		}

		{ // Step 2 : Create Window
			// Convert "const char* title" to "wchar_t* title"
			std::unique_ptr<wchar_t[]> titleW;
			{
				const size_t length = std::strlen(title);
				titleW = std::make_unique<wchar_t[]>(length + 1u);
				mbstowcs(titleW.get(), title, length);
			}

			// Get The Desired Window Rect From The desired Client Rect
			RECT rect{ 0, 0, width, height };
			{
				if (WIN_FAILED(AdjustWindowRect(&rect, NULL, false)))
					WS_THROW("[WS] --> [WIN 32] Failed To Ajust Window Rect");
			}

			this->m_windowHandle = CreateWindowEx(
				0, L"Weiss's Windows Window Class",
				titleW.get(), WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT, rect.right - rect.left, rect.bottom - rect.top,
				NULL, NULL, hInstance, NULL
			);

			if (this->m_windowHandle == NULL) {
				WS_THROW("[WS] --> [WIN 32] Window Creation Failed");
			} else {
				this->m_bIsRunning = true;
			}
		}

		// Step 3 : Show Window
		ShowWindow(this->m_windowHandle, SW_SHOWNORMAL);

		// Step 4 : Set Window Pointer
		{
#ifdef __WEISS__PLATFORM_X64

			SetWindowLongPtr(this->m_windowHandle, GWLP_USERDATA, (LONG_PTR)this);

#else

			SetWindowLong(this->m_windowHandle, GWLP_USERDATA, (LONG)this);

#endif
		}
	}

	void Window::Show() WS_NOEXCEPT { ShowWindow(this->m_windowHandle, SW_SHOWNORMAL); }
	void Window::Hide() WS_NOEXCEPT { ShowWindow(this->m_windowHandle, SW_HIDE); }

	void Window::Close() WS_NOEXCEPT {
#ifdef __WEISS__DEBUG_MODE

		assert(this->m_windowHandle != NULL);

#endif // __WEISS__DEBUG_MODE

		if (WIN_FAILED(CloseWindow(this->m_windowHandle)))
			WS_THROW("[WS] --> [WIN 32] Failed To Close Window");

		this->m_windowHandle = NULL;
		this->m_bIsRunning   = false;
	}

	void Window::Minimize() WS_NOEXCEPT
	{
		ShowWindow(this->m_windowHandle, SW_MINIMIZE);
	}

	void Window::Update() WS_NOEXCEPT
	{
		this->m_mouse.BeginUpdate();

		MSG msg = { };
		while (PeekMessage(&msg, this->m_windowHandle, 0, 0, PM_REMOVE) > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	std::optional<LRESULT> Window::WinHandleMessage(UINT msg, WPARAM wParam, LPARAM lParam) WS_NOEXCEPT
	{
		switch (msg) {
		case WM_SIZE:
			return 0;
		case WM_DESTROY:
			this->Close();

			return 0;
		}

		std::optional<LRESULT> mouseOptional = this->m_mouse.WinHandleEvent(msg, wParam, lParam);
		if (mouseOptional.has_value()) return mouseOptional.value();

		std::optional<LRESULT> keyboardOptional = this->m_keyboard.WinHandleEvent(msg, wParam, lParam);
		if (keyboardOptional.has_value()) return keyboardOptional.value();

		return {  };
	}

#elif defined(__WEISS__OS_LINUX)

	Window::Window(const char* title, const uint16_t width, const uint16_t height) WS_NOEXCEPT
	{
		// Fetch Display
		this->m_pDisplayHandle = ::XOpenDisplay(0);

		// Create Window
		this->m_windowHandle = ::XCreateSimpleWindow(this->m_pDisplayHandle, DefaultRootWindow(this->m_pDisplayHandle),
												     0, 0, width, height, 0,
													 BlackPixel(this->m_pDisplayHandle, 0), WhitePixel(this->m_pDisplayHandle, 0));

		// Set Title
		::XSetStandardProperties(this->m_pDisplayHandle, this->m_windowHandle, title, title, None, NULL, 0, NULL);

		// Select Input Masks
		::XSelectInput(this->m_pDisplayHandle, this->m_windowHandle, __WEISS__XLIB_ALL_MASKS);

		this->Show();

		this->m_bIsRunning = true;
	}

	void Window::Show() WS_NOEXCEPT
	{
		::XMapWindow(this->m_pDisplayHandle, this->m_windowHandle);
		::XFlush(this->m_pDisplayHandle);
	}

	void Window::Hide() WS_NOEXCEPT
	{
		::XUnmapWindow(this->m_pDisplayHandle, this->m_windowHandle);
		::XFlush(this->m_pDisplayHandle);
	}

	void Window::Close() WS_NOEXCEPT
	{
		// Destroy Window & Close Display
		::XDestroyWindow(this->m_pDisplayHandle, this->m_windowHandle);
		::XCloseDisplay(this->m_pDisplayHandle);

		this->m_pDisplayHandle = nullptr;
		this->m_bIsRunning     = false;
	}

	void Window::Minimize() WS_NOEXCEPT
	{
		::XIconifyWindow(this->m_pDisplayHandle, this->m_windowHandle, 0);
		::XFlush(this->m_pDisplayHandle);
	}

	void Window::Update() WS_NOEXCEPT
	{
		this->m_mouse.BeginUpdate();

		this->m_mouse.LinUpdate(this->m_pDisplayHandle);
		this->m_keyboard.LinUpdate(this->m_pDisplayHandle);
	}

#endif

	Window::~Window() WS_NOEXCEPT
	{
		this->Close();
	}

}; // WS