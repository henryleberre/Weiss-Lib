#pragma once

#include "WSPch.h"

#ifdef __WEISS__OS_WINDOWS

	#define WS_IS_SOCKET_INVALID(sock) (sock == INVALID_SOCKET)
	#define WS_SET_SOCKET_TO_INVALID_STATE(sock) (sock = INVALID_SOCKET)
	#define WS_CLOSE_SOCKET(sock) (closesocket(sock))

#elif defined(__WEISS__OS_LINUX)

	#define WS_IS_SOCKET_INVALID(sock) (sock < 0)
	#define WS_SET_SOCKET_TO_INVALID_STATE(sock) (sock = -1)
	#define WS_CLOSE_SOCKET(sock) (close(sock))

#endif

#define WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(tcpValue, udpValue, bthValue) (std::is_same<SocketProtocol::TCP, _PROTOCOL>::value ? tcpValue : (std::is_same<SocketProtocol::UDP, _PROTOCOL>::value ? udpValue : (std::is_same<SocketProtocol::BTH, _PROTOCOL>::value ? bthValue : 0));) 

namespace WS {

#ifdef __WEISS__OS_WINDOWS

	typedef SOCKET WS_SOCKET_TYPE;

#elif defined(__WEISS__OS_LINUX)

	typedef int WS_SOCKET_TYPE;

#endif

	enum class SocketProtocol {
		TCP, // TCP
		UDP, // UDP
		BTH  // Bluetooth
	};

	template <SocketProtocol _PROTOCOL>
	class ClientSocket {
	private:
		WS_SOCKET_TYPE m_socket;

	public:
		ClientSocket() = default;

		bool Connect(const char* host, const uint16_t port) WS_NOEXCEPT;

		void Disconnect() WS_NOEXCEPT;

		~ClientSocket() WS_NOEXCEPT;
	};

	template <SocketProtocol _PROTOCOL>
	class ServerSocket {
	private:
		WS_SOCKET_TYPE m_socket;
	
	public:
		ServerSocket() = default;

		void Disconnect() WS_NOEXCEPT;

		~ServerSocket() WS_NOEXCEPT;
	};

}; // WS