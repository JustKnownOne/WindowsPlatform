#include "network.h"

Socket::Socket()
	: m_socket(INVALID_SOCKET)
	, ai(NULL)
	, bCreated(false)
	, bConnected(false)
	, bBounded(false)
{
	std::clog << "Socket ctor" << std::endl;
}
Socket::Socket(const Socket& o)
	: m_socket(o.m_socket)
	, ai(o.ai)
	, bCreated(o.bCreated)
	, bConnected(o.bConnected)
	, bBounded(o.bBounded)
{
	std::clog << "Socket copy ctor" << std::endl;
}
Socket::~Socket()
{
	std::clog << "Socket dtor" << std::endl;
}

Socket& Socket::operator = (const Socket& o)
{
	std::clog << "Socket assign operator" << std::endl;

	m_socket = o.m_socket;
	ai = o.ai;
	bCreated = o.bCreated;
	bConnected = o.bConnected;
	bBounded = o.bBounded;

	return *this;
}

bool Socket::Open(ServerInfo& si)
{
	std::clog << "Socket::Open start" << std::endl;

	addrinfo* results = NULL;
	addrinfo hints;

	::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = si.family;
	hints.ai_socktype = si.socktype;
	hints.ai_protocol = si.protocol;
	hints.ai_flags = si.flags;

	int ret;
	ret = ::getaddrinfo(si.ip, si.port, &hints, &results);
	if (ret != ERROR_SUCCESS)
	{
		std::cerr << "::getaddrinfo failed : " << ret << std::endl;
		return false;
	}

	ai = results;

	m_socket = ::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (m_socket == INVALID_SOCKET)
	{
		std::cerr << "::socket failed : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	bCreated = true;

	std::clog << "Socket::Open end" << std::endl;
	return true;
}
bool Socket::Connect()
{
	std::clog << "Socket::Connect start" << std::endl;

	if (!bCreated)
	{
		std::cerr << "Socket do not created." << std::endl;
		return false;
	}

	int ret = ::connect(m_socket, ai->ai_addr, (int)ai->ai_addrlen);
	if (ret != ERROR_SUCCESS)
	{
		std::cerr << "::connect failed : " << ::WSAGetLastError() << std::endl;

		::freeaddrinfo(ai);
		ai = NULL;

		return false;
	}

	::freeaddrinfo(ai);
	ai = NULL;

	bConnected = true;

	std::clog << "Socket::Connect end" << std::endl;
	return true;
}
bool Socket::Bind()
{
	std::clog << "Socket::Bind start" << std::endl;

	if (!bCreated)
	{
		std::cerr << "Socket do not created." << std::endl;
		return false;
	}

	if (bConnected)
	{
		std::cerr << "Socket is connected. connected socket can not bind." << std::endl;
		return false;
	}

	int ret = ::bind(m_socket, ai->ai_addr, (int)ai->ai_addrlen);
	if (ret != ERROR_SUCCESS)
	{
		std::cerr << "::bind failed : " << ::WSAGetLastError() << std::endl;

		::freeaddrinfo(ai);
		ai = NULL;

		return false;
	}

	::freeaddrinfo(ai);
	ai = NULL;

	bBounded = true;

	std::clog << "Socket::Bind end" << std::endl;
	return true;
}
bool Socket::Close()
{
	std::clog << "Socket::Close start" << std::endl;

	::closesocket(m_socket);

	std::clog << "Socket::Close end" << std::endl;
	return true;
}

bool Socket::Send(const char* buffer, const unsigned& size)
{
	std::clog << "Socket::Send start" << std::endl;

	if (!bConnected)
	{
		std::cerr << "socket do not connected." << std::endl;
		return false;
	}

	int ret = ::send(m_socket, buffer, size, 0);
	if (ret == SOCKET_ERROR)
	{
		std::cerr << "::send failed : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	std::clog << ret << "bytes sent." << std::endl;

	std::clog << "Socket::Send end" << std::endl;
	return true;
}
bool Socket::Recv(char* buffer, unsigned& size)
{
	std::clog << "Socket::Recv start" << std::endl;

	if (!bConnected)
	{
		std::cerr << "socket do not connected." << std::endl;
		return false;
	}

	int ret = ::recv(m_socket, buffer, size, 0);
	if (ret == SOCKET_ERROR)
	{
		std::cerr << "::recv failed : " << ::WSAGetLastError() << std::endl;
		return false;
	}
	else if (ret == 0)
	{
		std::cerr << "connection closed." << std::endl;
		Close();
		return false;
	}

	size = ret;
	std::clog << ret << "bytes received." << std::endl;

	std::clog << "Socket::Recv end" << std::endl;
	return true;
}
