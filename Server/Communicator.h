#pragma once
#include "Singleton.h"
#include "LoginRequestHandler.h"
#include "global.h"
#include <WinSock2.h>
#include <map>
#include <exception>
#include <iostream>
#include <thread>
#include <string>
#include "OTPCryptoAlgorithm.h"
using std::map;
using std::cout;
using std::endl;
constexpr int MAX_SIZE = 1024;
constexpr int PORT = 8265;
constexpr int IFACE = 0;



class Communicator : public Singleton
{
public:
	~Communicator();

	//Canceling copy constructor and copy operator
	Communicator(Communicator& other) = delete;
	void operator=(const Communicator& other) = delete;

	static Communicator* getInstance(RequestHandlerFactory* handlerFactory);
	//Function
	void startHandleRequests();
	string getKey(SOCKET socket, char* buffer );

protected:
	//C'tor
	Communicator(RequestHandlerFactory* handlerFactory);
private:
	//Help functions
	void bindAndListen();
	void handleNewClient(SOCKET socket);
	static Buffer* getDataFromBuffer(const Buffer& buf);
	static int getCode(const Buffer& buffer);
	//Singleton fields
	static Communicator* m_instance;
	static Lock m_lock;

	//Fields
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory* m_handlerFactory;
	map<SOCKET, string> m_keys;
	OTPCryptoAlgorithm* algo;
};
