#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <IPHlpApi.h>
#include <WS2tcpip.h>

#include <Windows.h>
#include <tchar.h>

#include <iostream>
#include <list>

#include "Thread.h"
#include "utility.hpp"

struct ClientInfo;
struct ServerInfo;

class ISocket
{
public :
	ISocket();
	virtual ~ISocket();

private :
	WSADATA wsaData;
};

struct IProtocol
{
};

struct TCP : public IProtocol
{
	static const int protocol = IPPROTO_TCP;
	static const int socktype = SOCK_STREAM;
};

struct UDP : public IProtocol
{
	static const int protocol = IPPROTO_UDP;
	static const int socktype = SOCK_DGRAM;
};

class Socket
{
public :
	Socket();
	Socket(const Socket& o);
	~Socket();

	Socket& operator = (const Socket& o);

	bool Open(ServerInfo& si);
	bool Connect();
	bool Bind();
	bool Close();

	bool Send(const char* buffer, const unsigned& size);
	bool Recv(char* buffer, unsigned& size);

	inline const SOCKET& GetSocket() const
	{
		return m_socket;
	}
	inline const SOCKET& SetSocket(const SOCKET& s) const
	{
		m_socket = s;

		return m_socket;
	}
private :
	bool bCreated;
	bool bConnected;
	bool bBounded;

	mutable SOCKET m_socket;
	addrinfo* ai;
	static ISocket sc_init;
};

struct ClientInfo
{
	Socket line;
};

struct ServerInfo
{
	char* ip;
	char* port;

	Socket* line;

	int family;
	int socktype;
	int protocol;
	int flags;

	ServerInfo():ip(0), port(0), line(0){}
	~ServerInfo()
	{
		::SafeDeleteArray(ip);
		::SafeDeleteArray(port);
	}
};

class NetworkNode
{
public :
	NetworkNode(){}
	virtual ~NetworkNode(){}
};

class Server : public NetworkNode
{
public :
	Server();
	~Server();

	bool Open(const char* szPort);
	bool Close();

	bool Ready();

	bool AllocSocket();

protected :
	static unsigned __stdcall Todo(void* arg);
protected :
	Socket openline;
	std::list<ClientInfo*> clientlist;
};

class Client : public NetworkNode
{
public :
	Client();
	~Client();

	bool Connect(const char* ip, const char* port);
	bool Connect(ServerInfo& si);
	bool Close();

	bool IsAlive() const;

protected :
	static unsigned __stdcall Todo(void* arg);
protected :
	bool bAlive;
	Socket connection;
};

template<typename Data>
class Packet : public Data
{
public :
	Packet();
	virtual ~Packet();

	void SetSocket(const Socket& s);

	bool Receive()
	{
		if (m_socket.GetSocket() == INVALID_SOCKET)
			return false;

		return false;
	}
	bool Send()
	{
		if (m_socket.GetSocket() == INVALID_SOCKET)
			return false;

		return false;
	}
private :
	Socket m_socket;
};

struct tcp_test
{
	short cbSize;
	BYTE	type;
	BYTE* data;
};
