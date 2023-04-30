#pragma once
#include "LoginRequestHandler.h"
#include <WinSock2.h>
#include <map>
#include <exception>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

using std::map;
using std::cout;
using std::endl;
constexpr int MAX_SIZE = 1024;
constexpr int PORT = 8265;
constexpr int IFACE = 0;



class Communicator
{
public:
	Communicator();
	void startHandleRequests();
private:
	void bindAndListen();
	void handleNewClient(SOCKET socket);


	SOCKET m_serverSocket;
	map<SOCKET, IrequestHandler*> m_clients;

	char buffer[MAX_SIZE] = { 0 };//init buffer
};
