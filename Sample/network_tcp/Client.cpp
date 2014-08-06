#include "network.h"

Client::Client()
{
}
Client::~Client()
{
}

bool Client::Connect(const char* ip, const char* port)
{
	ServerInfo si;
	::strAlloc(si.ip, ip);
	::strAlloc(si.port, port);
	si.family = AF_INET;
	si.socktype = SOCK_STREAM;
	si.protocol = IPPROTO_TCP;
	si.flags = 0;

	if (!Connect(si))
	{
		return false;
	}

	return true;
}
bool Client::Connect(ServerInfo& si)
{
	if (!connection.Open(si))
	{
		return false;
	}

	if (!connection.Connect())
	{
		return false;
	}

	ThreadInfo<ServerInfo>* info = new ThreadInfo<ServerInfo>;
	info->data.line = &connection;
	info->hThread = (HANDLE)::_beginthreadex(NULL, 0, &Client::Todo, (void*)info, 0, &info->uId);

	ClientThreadDepot.push(info);

	return true;
}
bool Client::Close()
{
	return connection.Close();
}

bool Client::IsAlive() const
{
	return bAlive;
}

unsigned __stdcall Client::Todo(void* arg)
{
	ThreadInfo<ServerInfo>* info = (ThreadInfo<ServerInfo>*)arg;

	info->data.line->Send("ack", 3);

	char buffer[100];
	unsigned size = 100;
	info->data.line->Recv(buffer, size);

	return 0;
}