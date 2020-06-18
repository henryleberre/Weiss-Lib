#include "WSPeripheral.h"

namespace WS {

	void Mouse::BeginUpdate() WS_NOEXCEPT
	{
		this->m_deltaPosition = { 0, 0 };
		this->m_wheelDelta = 0;
	}

#ifdef __WEISS__OS_WINDOWS

	Mouse::Mouse() WS_NOEXCEPT
	{
		RAWINPUTDEVICE mouseInputDevice;
		mouseInputDevice.usUsagePage = 0x01;
		mouseInputDevice.usUsage = 0x02;
		mouseInputDevice.dwFlags = 0;
		mouseInputDevice.hwndTarget = nullptr;

		if (WIN_FAILED(RegisterRawInputDevices(&mouseInputDevice, 1, sizeof(RAWINPUTDEVICE))))
			WS_THROW("[WS] --> [WIN RAW INPUT] Failed To Register Mouse Raw Input Device");
	}

	Mouse::~Mouse() WS_NOEXCEPT
	{

	}

	std::optional<LRESULT> Mouse::WinHandleEvent(const UINT msg, const WPARAM wParam, const LPARAM lParam) WS_NOEXCEPT
	{
		switch (msg)
		{
		case WM_INPUT:
		{
			UINT size = 0;

			if (!GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)))
			{
				std::vector<char> rawBuffer(size);

				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawBuffer.data(), &size, sizeof(RAWINPUTHEADER)) == size)
				{
					const RAWINPUT& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());

					if (ri.header.dwType == RIM_TYPEMOUSE && (ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
					{
						this->m_deltaPosition.x += static_cast<int16_t>(ri.data.mouse.lLastX);
						this->m_deltaPosition.y += static_cast<int16_t>(ri.data.mouse.lLastY);
					}
				}
			}

			return 0;
		}
		case WM_MOUSEMOVE:
			this->m_position = { static_cast<uint16_t>(GET_X_LPARAM(lParam)),
								 static_cast<uint16_t>(GET_Y_LPARAM(lParam)) };

			return 0;
		case WM_LBUTTONDOWN:
			this->m_bIsLeftButtonDown = true;

			return 0;
		case WM_LBUTTONUP:
			this->m_bIsLeftButtonDown = false;

			return 0;
		case WM_RBUTTONDOWN:
			this->m_bIsRightButtonDown = true;

			return 0;
		case WM_RBUTTONUP:
			this->m_bIsRightButtonDown = false;

			return 0;
		case WM_MOUSEWHEEL:
			this->m_wheelDelta += GET_WHEEL_DELTA_WPARAM(wParam);

			return 0;
		default:
			return {  };
		}
	}

#elif defined(__WEISS__OS_LINUX)

	void Mouse::LinUpdate(::Display* pDisplayHandle) WS_NOEXCEPT
	{
		::XEvent xEvent;
		while (XCheckMaskEvent(pDisplayHandle, __WEISS__XLIB_MOUSE_MASKS, &xEvent)) {
			switch (xEvent.type) {
			case ButtonPress:
				switch (xEvent.xbutton.button) {
				case Button1:
					this->m_bIsLeftButtonDown = true;
					break;
				case Button3:
					this->m_bIsRightButtonDown = true;
					break;
				case Button4:
					this->m_wheelDelta += xEvent.xbutton.y;
					break;
				case Button5:
					this->m_wheelDelta -= xEvent.xbutton.y;
					break;
				}

				this->m_position = { static_cast<uint16_t>(xEvent.xmotion.x),
									 static_cast<uint16_t>(xEvent.xmotion.y) };

				break;
			case ButtonRelease:
				switch (xEvent.xbutton.button) {
				case 1:
					this->m_bIsLeftButtonDown = false;
					break;
				case 3:
					this->m_bIsRightButtonDown = false;
					break;
				}

				this->m_position = { static_cast<uint16_t>(xEvent.xmotion.x),
									 static_cast<uint16_t>(xEvent.xmotion.y) };

				break;
			case MotionNotify:
				this->m_position = { static_cast<uint16_t>(xEvent.xmotion.x),
									 static_cast<uint16_t>(xEvent.xmotion.y) };

				break;
			}
		}
	}

#endif


#ifdef __WEISS__OS_WINDOWS

	std::optional<LRESULT> Keyboard::WinHandleEvent(const UINT msg, const WPARAM wParam, const LPARAM lParam) WS_NOEXCEPT
	{
		switch (msg)
		{
		case WM_KEYDOWN:
			this->m_downKeys[static_cast<uint8_t>(wParam)] = true;

			return 0;

		case WM_KEYUP:
			this->m_downKeys[static_cast<uint8_t>(wParam)] = false;

			return 0;
		}

		return {  };
	}

#elif defined(__WEISS__OS_LINUX)

	void Keyboard::LinUpdate(::Display* pDisplayHandle) WS_NOEXCEPT
	{
		::XEvent xEvent;
		while (XCheckMaskEvent(pDisplayHandle, __WEISS__XLIB_KEYBOARD_MASKS, &xEvent)) {
			const char key = std::toupper(XLookupKeysym(&xEvent.xkey, 0));
			switch (xEvent.type) {
			case KeyPress:
				this->m_downKeys[key] = true;
				break;
			case KeyRelease:
				this->m_downKeys[key] = false;
				break;
			}
		}
	}

#endif

}; // WS