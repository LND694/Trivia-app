#include "Communicator.h"


std::mutex mtx;


Communicator::Communicator()
{

	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - init socket");
}

void Communicator::startHandleRequests()
{

	SOCKET client_socket = NULL;
	bindAndListen();//start listening
	while (true)//look up for connections
	{
		client_socket = accept(this->m_serverSocket, NULL, NULL);
		if (client_socket != INVALID_SOCKET)
			cout << "Client accepted !" << endl;
		// create new thread to handle the new client for client	and detach from it
		std::thread handle(&Communicator::handleNewClient,this, client_socket);
		handle.detach();
	}
}

void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	cout << "binded" << endl;

	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");

	cout << "listening... " << endl;
}

void Communicator::handleNewClient(SOCKET socket)
{
	int len = 0;//the length of the recieved message
	std::lock_guard<std::mutex> lck(mtx);//prevent out of sync messages
	this->m_clients.insert({ socket, new LoginRequestHandler() });//init a new pair of the given socket and a login request since it is a new user
	send(socket, "hello", sizeof("hello"), 0);
	cout << "Hello message sent " << endl;
	len = recv(socket, buffer, MAX_SIZE - 1, NULL);//MAX_SIZE-1 forthe null terminator

	buffer[len] = 0;//add null terminator

	if (len > 0)
	{
		cout << "message from client: " << buffer << endl;
	}
	else
	{
		cout << "message from the client is empty" << endl;
	}
}
