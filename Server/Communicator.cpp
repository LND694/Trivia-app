#include "Communicator.h"


Communicator* Communicator::m_instance = nullptr;
Lock Communicator::m_lock;

/// <summary>
/// C'tor of class Communicator
/// </summary>
/// <param name="handlerFactory">The factory of the handlers of the requests.</param>
Communicator::Communicator(RequestHandlerFactory* handlerFactory):
	m_handlerFactory(handlerFactory)
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->m_serverSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - init socket");
}

/// <summary>
/// D'tor of class Communicator
/// </summary>
Communicator::~Communicator()
{

}

/// <summary>
/// The function getts the instance of the class Communicator.
/// </summary>
/// <param name="handlerFactory"> The RequestHandlerFactory of the Communicator.</param>
/// <returns> The instance of the class Communicator.</returns>
Communicator* Communicator::getInstance(RequestHandlerFactory* handlerFactory)
{
	lock_guard<Lock> lockGuard(m_lock);
	if (m_instance == nullptr)
	{
		m_instance = new Communicator(handlerFactory);
	}
	return m_instance;
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
	RequestResult res;
	RequestInfo info;
	LoginRequest logReq;
	SignupRequest signUpReq;
	string code;
	Buffer* data;
	this->m_clients.insert({ socket, this->m_handlerFactory->createLoginRequestHandler()});//init a new pair of the given socket and a login request since it is a new user
	while (this->m_clients.at(socket) != nullptr)
	{
		len = recv(socket, buffer, MAX_SIZE - 1, NULL);//MAX_SIZE-1 forthe null terminator

		Buffer charVector(buffer, buffer + MAX_SIZE);
		charVector[len] = '\0';//add null terminator

		data = getDataFromBuffer(charVector);
		//Extracting the code from the request's buffer
		for (int i = 0; i < SIZE_CODE_FIELD; i++)
		{
			code += data->at(i);
		}

		//turn the buffer into request
		info.buffer = *data;
		info.receivalTime = time(nullptr);//get the current time
		info.id = static_cast<RequestId>(atoi(code.c_str()));

		//get the response
		res = this->m_clients.at(socket)->handleRequest(info);
		cout << res.response.data() << endl;

		//send the response
		send(socket, reinterpret_cast<char*>(res.response.data()), static_cast<int>(res.response.size()), NULL);

		this->m_clients.at(socket) = res.newHandler;
		code = "";//reset code
	}
}

/// <summary>
/// The function getts the segment of the data 
/// and inserting it into another Buffer.
/// </summary>
/// <param name="buf"> The Buffer of the full request.</param>
/// <returns> a pointer to a Buffer variable- the data only.</returns>
Buffer* Communicator::getDataFromBuffer(const Buffer& buf)
{
	Buffer* data = new Buffer();
	unsigned char currentChar = 0;

	//Going over the buffer, starting from the data segment
	for (int i = 0; i < buf.size(); i++)
	{
		currentChar = buf[i];
		//if the character is not a letter or a scope
		if (currentChar != SPACE && currentChar != NEW_LINE && currentChar != END_STR_SYMBOL)
		{
			data->push_back(currentChar);
		}
	}
	return data;
}