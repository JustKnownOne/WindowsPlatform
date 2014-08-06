#pragma once

#include <Windows.h>
#include <process.h>
#include <list>
#include "utility.hpp"

template<typename Data>
struct ThreadInfo
{
	HANDLE hThread;
	unsigned uId;
	Data data;
};

template<typename Info>
class ThreadManager : public singleton<ThreadManager<Info>>
{
	friend class singleton<ThreadManager<Info>>;
	typedef Info* PointerType;
	typedef Info& ReferenceType;
	typedef PointerType& PointerRefType;
	typedef std::list<PointerType> DepotType;
private :
	ThreadManager(){}
	~ThreadManager(){}

public :
	void push(const PointerRefType data)
	{
		depot.push_back(data);
	}

private :
	DepotType depot;
};

#define ServerThreadDepot ThreadManager<ThreadInfo<ClientInfo*>>::GetReference()
#define ClientThreadDepot ThreadManager<ThreadInfo<ServerInfo>>::GetReference()