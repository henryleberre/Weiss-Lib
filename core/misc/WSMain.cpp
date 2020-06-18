#include "WSMain.h"

int main(int argc, char** argv)
{
#ifdef __WEISS__OS_WINDOWS

	if (WIN_FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)))
		WS_THROW("[COM] Failed To CoInitialize");

	WSADATA wsaData;
	if (WIN_FAILED(WSAStartup(MAKEWORD(2, 0), &wsaData)))
		WS_THROW("[WINSOCK] Failed To WSAStartup");

#endif // __WEISS__OS_WINDOWS

	const int returnCode = WS::EntryPoint(argc, argv);

#ifdef __WEISS__OS_WINDOWS

	if (WIN_FAILED(WSACleanup()))
		WS_THROW("[WINSOCK] Failed To WSACleanup");

	CoUninitialize();

#endif // __WEISS__OS_WINDOWS

	return returnCode;
}