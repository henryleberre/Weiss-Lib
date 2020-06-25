#include "WSSocket.h"

namespace WS {

	template <SocketProtocol _PROTOCOL>
	void SocketBase<_PROTOCOL>::Disconnect() WS_NOEXCEPT
	{
		WS_CLOSE_SOCKET(this->m_socket);

		WS_SET_SOCKET_TO_INVALID_STATE(this->m_socket);
	}

	template <SocketProtocol _PROTOCOL>
	SocketBase<_PROTOCOL>::~SocketBase() WS_NOEXCEPT
	{
		this->Disconnect();
	}

	template <SocketProtocol _PROTOCOL>
	[[nodiscard]] bool ClientSocket<_PROTOCOL>::Connect(const char* host, const uint16_t port) const WS_NOEXCEPT
	{
		sockaddr_in sockAddrIn;
		sockAddrIn.sin_addr.s_addr = inet_addr(host);
		sockAddrIn.sin_family      = AF_INET;
		sockAddrIn.sin_port        = htons(port);
																						   // TCP       // UDP
		constexpr const int addressFamily = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, AF_INET,     AF_INET);
		constexpr const int type          = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, SOCK_STREAM, SOCK_DGRAM);
		constexpr const int protocol      = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, IPPROTO_TCP, IPPROTO_UDP);

		this->m_socket = socket(addressFamily, type, protocol);

		if (WS_IS_SOCKET_INVALID(this->m_socket)) {
			WS_THROW("[WS] Failed To Open Socket");
			return false;
		}

		if (WS_SOCKET_FAILED(connect(this->m_socket, (sockaddr*)&sockAddrIn, sizeof(sockAddrIn)))) {
			WS_THROW("[WS] Failed To Connect Client Socket To Server Socket");
			return false;
		}

		return true;
	}

	template <SocketProtocol _PROTOCOL>
	[[nodiscard]] bool ServerSocket<_PROTOCOL>::Bind(const uint16_t port) const WS_NOEXCEPT
	{
		sockaddr_in sockAddr;
		sockAddr.sin_addr.s_addr = INADDR_ANY;
		sockAddr.sin_family      = AF_INET;
		sockAddr.sin_port        = htons(port);

		if (WS_SOCKET_FAILED(bind(this->m_socket, (sockaddr*)&sockAddr, sizeof(sockAddr)))) {
			WS_THROW("[SERVER SOCKET] Binding Failed");
			return false;
		}

		return true;
	}

	template <SocketProtocol _PROTOCOL>
	[[nodiscard]] bool ServerSocket<_PROTOCOL>::Listen() const WS_NOEXCEPT
	{
		if (WS_SOCKET_FAILED(listen(this->m_socket, SOMAXCONN))) {
			WS_THROW("[SERVER SOCKET] Listening Failed");
			return false;
		}

		return true;
	}

}; // WS