#include "network.h"

ISocket Socket::sc_init;

ISocket::ISocket()
{
	std::clog << "Winsock initialize..." << std::endl;

	int iResult;
	iResult = ::WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != ERROR_SUCCESS)
	{
		std::cerr << "Winsock Dll initailzation failed : " << iResult << std::endl;
	}

	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2)
	{
		std::cerr << "Winsock initialization failed. usable version not found." << std::endl;
	}
}
ISocket::~ISocket()
{
	std::clog << "Winsock release..." << std::endl;

	::WSACleanup();
}