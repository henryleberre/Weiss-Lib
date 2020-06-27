#include "WSPeripheral.h"

namespace WS {

#ifdef __WEISS__OS_WINDOWS
		Mouse::Mouse() WS_NOEXCEPT {
            RAWINPUTDEVICE mouseInputDevice;
            mouseInputDevice.usUsagePage = 0x01;
            mouseInputDevice.usUsage = 0x02;
            mouseInputDevice.dwFlags = 0;
            mouseInputDevice.hwndTarget = nullptr;

            if (WIN_FAILED(RegisterRawInputDevices(&mouseInputDevice, 1, sizeof(RAWINPUTDEVICE))))
                WS_THROW("[WS] --> [WIN RAW INPUT] Failed To Register Mouse Raw Input Device");
        }
#endif

}; // WS