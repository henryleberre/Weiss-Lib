#include "WSSocket.h"

namespace WS {

	template <SocketProtocol _PROTOCOL>
	bool ClientSocket<_PROTOCOL>::Connect(const char* host, const uint16_t port) WS_NOEXCEPT
	{
		sockaddr_in sockAddrIn;
		sockAddrIn.sin_addr.s_addr = inet_addr(host);
		sockAddrIn.sin_family      = AF_INET;
		sockAddrIn.sin_port        = htons(port);
																				// TCP       // UDP       // BTH
		constexpr const int addressFamily = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, AF_INET,     AF_INET,     AF_BLUETOOTH);
		constexpr const int type          = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, SOCK_STREAM, SOCK_DGRAM,  SOCK_STREAM);
		constexpr const int protocol      = WS_SOCKET_SELECT_VALUE_PER_PROTOCOL(_PROTOCOL, IPPROTO_TCP, IPPROTO_UDP, 0); // BTHPROTO_RFCOMM

		this->m_socket = socket(addressFamily, type, protocol);

		if (WS_IS_SOCKET_INVALID(this->m_socket)) {
			WS_THROW("[WS] Failed To Open Socket");
			return false;
		}

		if (connect(this->m_socket, (sockaddr*)&sockAddrIn, sizeof(sockAddrIn)) < 0) {
			WS_THROW("[WS] Failed To Connect Client Socket To Server Socket");
			return false;
		}

		return true;
	}

	template <SocketProtocol _PROTOCOL>
	void ClientSocket<_PROTOCOL>::Disconnect() WS_NOEXCEPT
	{
		WS_CLOSE_SOCKET(this->m_socket);

		WS_SET_SOCKET_TO_INVALID_STATE(this->m_socket);
	}

	template <SocketProtocol _PROTOCOL>
	ClientSocket<_PROTOCOL>::~ClientSocket() WS_NOEXCEPT
	{
		this->Disconnect();
	}

}; // WS