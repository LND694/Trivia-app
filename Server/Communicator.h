#pragma once
#include "LoginRequestHandler.h"
#include <WinSock2.h>
#include <map>
#include <exception>
#include <iostream>
#include <thread>
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
	//C'tor
	Communicator(RequestHandlerFactory& handlerFactory);

	void startHandleRequests();
private:
	//Help functions
	void bindAndListen();
	void handleNewClient(SOCKET socket);

	//Fields
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory& m_handlerFactory;

};
