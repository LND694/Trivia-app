#include "Communicator.h"


/// <summary>
/// C'tor of the class Communicator.
/// </summary>
Communicator::Communicator()
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - init socket");
}

/// <summary>
/// The function starts to look for requests of clients and accept them.
/// </summary>
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
/// <summary>
/// The function binds the Socket of the server and starts to 
/// listen to requests.
/// </summary>
void Communicator::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;

	//Binding the Socket of the server to the number of the port
	if (bind(this->m_serverSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	cout << "binded" << endl;

	//Starting to listen to requsets from another user without accepting them
	if (listen(this->m_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");

	cout << "listening... " << endl;
}
/// <summary>
/// The function communicates with a single client only.
/// </summary>
/// <param name="socket">The Socket of the client to handle.</param>
void Communicator::handleNewClient(SOCKET socket)
{
	int len = 0;//the length of the recieved message
	char buffer[MAX_SIZE] = { 0 };
	this->m_clients.insert({ socket, new LoginRequestHandler() });//init a new pair of the given socket and a login request since it is a new user
	send(socket, "hello", sizeof("hello"), 0);
	cout << "Hello message sent " << endl;
	len = recv(socket, buffer, MAX_SIZE - 1, NULL);//MAX_SIZE-1 forthe null terminator

	buffer[len] = '\0';//add null terminator

	//Printing the recived message
	if (len > 0)
	{
		cout << "message from client: " << buffer << endl;
	}
	else
	{
		cout << "message from the client is empty" << endl;
	}

}
