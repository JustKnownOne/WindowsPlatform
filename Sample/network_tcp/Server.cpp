#include "network.h"

Server::Server()
{
	std::clog << "Server ctor" << std::endl;
}
Server::~Server()
{
	std::clog << "Server dtor" << std::endl;
}

bool Server::Open(const char* szPort)
{
	std::clog << "Server::Open start" << std::endl;

	ServerInfo si;
	::strAlloc(si.port, szPort);
	si.family = AF_INET;
	si.socktype = SOCK_STREAM;
	si.protocol = IPPROTO_TCP;
	si.flags = AI_PASSIVE;

	if (!openline.Open(si))
	{
		return false;
	}

	if (!openline.Bind())
	{
		return false;
	}

	int ret = ::listen(openline.GetSocket(), SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		std::cerr << "::listen failed : " << ::WSAGetLastError() << std::endl;
		return false;
	}

	std::clog << "Server::Open end" << std::endl;

	return true;
}
bool Server::Close()
{
	std::clog << "Server::Close start" << std::endl;

	return openline.Close();
}
bool Server::Ready()
{
	std::clog << "Server::Ready start" << std::endl;

	ClientInfo* ci = new ClientInfo;
	if (ci->line.SetSocket(::accept(openline.GetSocket(), NULL, NULL)) == INVALID_SOCKET)
	{
		std::cerr << "::accept failed : " << ::WSAGetLastError() << std::endl;

		delete ci;

		return false;
	}

	clientlist.push_back(ci);

	std::clog << "Server::Ready end" << std::endl;
	return true;
}
bool Server::AllocSocket()
{
	ThreadInfo<ClientInfo*>* info = new ThreadInfo<ClientInfo*>;
	info->data = *(clientlist.rbegin());
	info->hThread = (HANDLE)::_beginthreadex(NULL, 0, &Server::Todo, (void*)info, 0, &info->uId);

	ServerThreadDepot.push(info);

	return true;
}
unsigned __stdcall Server::Todo(void* arg)
{
	ThreadInfo<ClientInfo*>* info = (ThreadInfo<ClientInfo*>*)arg;

	info->data->line.Send("check", 5);

	char buffer[100];
	unsigned size = 100;
	info->data->line.Recv(buffer, size);

	return 0;
}