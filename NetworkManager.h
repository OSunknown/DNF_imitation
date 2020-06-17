#pragma once
#include <string>
using namespace std;
#include <ws2tcpip.h>

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();
	static NetworkManager& inse() { static NetworkManager instance; return instance; }
public:
	int Connect();
	void Disconnect();
	void Send(string msg);
	void Send(wchar_t* msg);
public:
	SOCKET listenSocket;
};

