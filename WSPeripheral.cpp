#include "WSPeripheral.h"

namespace WS {

	void Mouse::BeginUpdate() WS_NOEXCEPT
	{
		this->m_deltaPosition = { 0, 0 };
		this->m_wheelDelta    = 0;

#ifdef __WEISS__OS_LINUX

		this->m_bIsLeftButtonDown  = false;
		this->m_bIsRightButtonDown = false;

#endif
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

	std::optional<LRESULT> Mouse::HandleEvent(const UINT msg, const WPARAM wParam, const LPARAM lParam) WS_NOEXCEPT
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
		case WM_MOUSELEAVE:
			this->m_bIsCursorInWindow;

			return 0;
		case WM_MOUSEMOVE:
			this->m_position = { static_cast<uint16_t>(GET_X_LPARAM(lParam)), static_cast<uint16_t>(GET_Y_LPARAM(lParam)) };

			this->m_bIsCursorInWindow = true;

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

	bool Mouse::HandleEvent(const XEvent& xEvent) WS_NOEXCEPT
	{
		switch (xEvent.type) {
		case ButtonPressMask:
			switch (xEvent.xbutton.button) {
			case 1:
				this->m_bIsLeftButtonDown = true;
				break;
			case 3:
				this->m_bIsRightButtonDown = true;
				break;
			}
			return true;

		default:
			return false;
		}
	}

#endif

}; // WS