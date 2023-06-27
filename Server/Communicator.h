#pragma once
#include "LoginRequestHandler.h"
#include <WinSock2.h>
#include <map>
#include <exception>
#include <iostream>
#include <thread>
#include "OTPCryptoAlgorithm.h"
#include "AesEncryption.h"
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
	string getEncyptionData(SOCKET socket, char* buffer ) const;

protected:
	//C'tor
	Communicator(RequestHandlerFactory* handlerFactory);
private:
	//Help functions
	void bindAndListen();
	void handleNewClient(SOCKET socket);
	static Buffer* getDataFromBuffer(const Buffer& buf);
	static int getCode(const Buffer& buffer);
	static RequestResult& createErrorResponse(const string errorMsg, IRequestHandler* lastHandler);
	//Singleton fields
	static Communicator* m_instance;
	static Lock m_lock;

	//Fields
	SOCKET m_serverSocket;
	map<SOCKET, IRequestHandler*> m_clients;
	RequestHandlerFactory* m_handlerFactory;
	map<SOCKET, string> m_keys;
	map<SOCKET, string> m_ivs;
	CryptoAlgorithm* algo;
};
