#pragma once

#ifdef _WIN32

	#define __WEISS__OS_WINDOWS

	#ifdef _WIN64

		#define __WEISS__PLATFORM_X64

	#else // End Of #ifdef _WIN64

		#define __WEISS__PLATFORM_X86

	#endif

	// Vulkan
	#define VK_USE_PLATFORM_WIN32_KHR

	#define _CRT_SECURE_NO_WARNINGS         // In order to use mbstowcs
	#define _WINSOCKAPI_					// Stops Windows.h from including winsock2
	#define _WINSOCK_DEPRECATED_NO_WARNINGS // Lets us use htons()
	#define NOMINMAX

	// Windows Includes
	#include <wrl.h>
	#include <strsafe.h>
	#include <Windows.h>
	#include <windowsx.h>

	// Windows Networking Includes
	#include <ws2bth.h>
	#include <winhttp.h>
	#include <winsock2.h>
	#include <ws2tcpip.h>

	// Windows Image Includes
	#include <wincodec.h>

	// Windows DirectX 11 Includes
	#include <d3d11.h>

	// Windows DirectX 12 Includes
	#include <d3d12.h>
	//#include <d3dx12.h> // from https://github.com/Microsoft/DirectX-Graphics-Samples/tree/master/Libraries/D3DX12
	#include <dxgi1_6.h>

	// Windows DirectX 11/12 Includes
	#include <d3dcompiler.h>

	// Link
	#pragma comment(lib, "dxgi.lib")
	#pragma comment(lib, "winmm.lib")
	#pragma comment(lib, "d3d11.lib")
	#pragma comment(lib, "d3d12.lib")
	#pragma comment(lib, "dxguid.lib")
	#pragma comment(lib, "user32.lib")
	#pragma comment(lib, "Ws2_32.lib")
	#pragma comment(lib, "Mswsock.lib")
	#pragma comment(lib, "winhttp.lib")
	#pragma comment(lib, "kernel32.lib")
	#pragma comment(lib, "AdvApi32.lib")
	#pragma comment(lib, "D3DCompiler.lib")
	#pragma comment(lib, "windowscodecs.lib")

#elif defined(__ANDROID__)

	#define __WEISS__OS_ANDROID

#elif defined(__linux__)

	#define __WEISS__OS_LINUX

	// Vulkan
	#define VK_USE_PLATFORM_XLIB_KHR

	// Sockets
	#include <netdb.h>
	#include <arpa/inet.h>
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>

	// Bluetooth
	#include <bluetooth/bluetooth.h>
	#include <bluetooth/rfcomm.h>
	#include <unistd.h>

	// X11 : For Linux Windows
	#include <X11/Xos.h>
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>

#elif defined(__APPLE__) && defined(__MACH__)

	#include <TargetConditionals.h>

	#if TARGET_IPHONE_SIMULATOR
		#define __WEISS__OS_IOS
	#elif TARGET_OS_IPHONE == 1
		#define __WEISS__OS_IOS
	#elif TARGET_OS_MAC == 1
		#define __WEISS__OS_OSX
	#else
		#error You Target Apple Device Could Not Be Resolved
	#endif

#else

	#error The Weiss Engine Could Not Determine Your Target OS

#endif

#if defined(_NDEBUG) || defined(NDEBUG)

	#define __WEISS__RELEASE_MODE

#else // end of #if defined(_NDEBUG) || defined(NDEBUG)

	#define __WEISS__DEBUG_MODE

#endif

// Vulkan Includes
//#include <vulkan/vulkan.h>

//#pragma comment(lib, "vulkan-1.lib")

/*
 * // ////////////////////////////--\\\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
 * // |/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
 * // ||------------------C++17 STL Includes------------------|| \\
 * // |\______________________________________________________/| \\
 * // \\\\\\\\\\\\\\\\\\\\\\\\\\\\--//////////////////////////// \\
 */

#include <bit>
#include <map>
#include <list>
#include <array>
#include <queue>
#include <mutex>
#include <cmath>
#include <future>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <bitset>
#include <cassert>
#include <cstring>
#include <numeric>
#include <codecvt>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <optional>
#include <exception>
#include <algorithm>
#include <functional>

#ifndef __WEISS__DISABLE_SIMD

	#include <immintrin.h>

#endif // #ifndef __WEISS__DISABLE_SIMD

/*
 * // //////////////////////////-\\\\\\\\\\\\\\\\\\\\\\\\\\ \\
 * // |/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
 * // ||-------------TO THROW OR NOT TO THROW------------|| \\
 * // |\_________________________________________________/| \\
 * // \\\\\\\\\\\\\\\\\\\\\\\\\\-////////////////////////// \\
 *
 * The use of noexcept can speed up execution but exceptions are needed in debug mode
 * The "WS_THROW" macro can be used to track exceptions even in release mode (i.e logging..)
 */

#ifdef __WEISS__DEBUG_MODE

	#define __WEISS__THROWS true

#else

	#define __WEISS__THROWS false

#endif

#ifdef __WEISS__THROWS == true

	#define WS_THROW(errorMsg) (throw std::runtime_error(errorMsg))

#else

	#define WS_THROW(errorMsg) {}

#endif

#define WS_NOEXCEPT noexcept(!__WEISS__THROWS)

/*
 * // //////////////////////-\\\\\\\\\\\\\\\\\\\\\\ \\
 * // |/‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾‾\| \\
 * // ||--------------HELPER MACROS--------------|| \\
 * // |\_________________________________________/| \\
 * // \\\\\\\\\\\\\\\\\\\\\\-////////////////////// \\
 */

#define CHECK_BIT(var, pos) ((var) & (1 << (pos)))

#define VK_FAILED(vkResult) ((vkResult) != VK_SUCCESS)

#ifdef __WEISS__OS_WINDOWS

	#define DX_FAILED(hResult)  FAILED(hResult)
	#define WIN_FAILED(hResult) FAILED(hResult)

	#define WS_ERROR_MESSAGE(message) MessageBoxA(NULL, message, "Weiss Engine Error", MB_TOPMOST | MB_ICONERROR);

#endif // __WEISS__OS_WINDOWS
