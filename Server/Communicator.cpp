#include "Communicator.h"


Communicator* Communicator::m_instance = nullptr;
Lock Communicator::m_lock;

/// <summary>
/// C'tor of class Communicator
/// </summary>
/// <param name="handlerFactory">The factory of the handlers of the requests.</param>
Communicator::Communicator(RequestHandlerFactory* handlerFactory) :
	m_handlerFactory(handlerFactory)
{
	this->m_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	this->algo = new RSACryptoAlgorithm();
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
		// create new thread to handle the new client for client and detach from it
		std::thread handle(&Communicator::handleNewClient, this, client_socket);
		handle.detach();
	}
}

/// <summary>
/// get the key from the client
/// </summary>
/// <param name="socket"> the socket of the connection </param>
/// <param name="buffer"> the buffer to read from</param>
/// <returns> the key in hex as string</returns>
string Communicator::getKey(SOCKET socket, char* buffer) const
{
	int len = 0;
	string key;
	len = recv(socket, buffer, MAX_SIZE - 1, NULL);//MAX_SIZE-1 for the null terminator
	if (len <= 0)
	{
		throw std::exception("The client disconnected");
	}
	buffer[len] = '\0';//add null terminator
	key = string(buffer);//convert to string
	//Reseting the buffer
	for (int i = 0; i < MAX_SIZE; i++)
	{
		buffer[i] = END_STR_SYMBOL;
	}
	return key;
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
	int len = 0;
	char buffer[MAX_SIZE] = { 0 };
	byte buffer1[MAX_SIZE] = { 0 };
	RequestResult res;
	RequestInfo info;
	string username = "";
	Buffer* data = nullptr;
	time_t sendingTime{};
	string key = "";
	string plainText = "";
	string encryptedText = "";
	this->m_keys.insert({ socket,getKey(socket, buffer) });//init a new pair of socket and key recived by the client
	this->m_clients.insert({ socket, this->m_handlerFactory->createLoginRequestHandler()});//init a new pair of the given socket and a login request since it is a new user
	try 
	{
		while (this->m_clients.at(socket) != nullptr)
		{
			//Getting the response of the client and checking the respose time
			sendingTime = time(nullptr);
			len = recv(socket, buffer, MAX_SIZE - 1, NULL);//MAX_SIZE-1 for the null terminator
			for (int i = 0; i < len; i++)
			{
				buffer1[i] = buffer[i];
			}
			if (len <= 0)
			{
				throw std::exception("The client disconnected");
			}
			key = this->m_keys.at(socket);

			info.receivalTime = time(nullptr) - sendingTime; //the time for the response to come

			encryptedText = string(reinterpret_cast<char*>(buffer1), sizeof(buffer1));//byte array to string
			plainText = this->algo->decrypt(encryptedText, key);
			cout << "decrypted: " << plainText << endl;
			Buffer charVector = this->algo->convertToBuffer(plainText);


			data = getDataFromBuffer(charVector);

			//turn the buffer into request
			info.buffer = *data;
			info.id = static_cast<RequestId>(getCode(*data));

			//get the response
			res = this->m_clients.at(socket)->handleRequest(info);
			cout << res.response.data() << endl;

			if (LOGIN_RESP_CODE == getCode(res.response)) //if there was a login
			{
				//saving the username
				username = JsonRequestPacketDeserializer::deserializeLoginRequest(*data).username;
			}
			res.response = this->algo->convertToBuffer(this->algo->encrypt(this->algo->convertToString(res.response), key));//encrypt the response
			cout << "encrypted: " << res.response.data() << endl;
			//send the response
			send(socket, reinterpret_cast<char*>(res.response.data()), static_cast<int>(res.response.size()), NULL);

			//Reseting variables
			delete data;
			this->m_clients.at(socket) = res.newHandler;
			len = 0;

			//Reseting the buffer
			for (int i = 0; i < MAX_SIZE; i++)
			{
				buffer[i] = END_STR_SYMBOL;
			}
		}
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		this->m_handlerFactory->getLoginManager()->logOut(username);
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

/// <summary>
/// The function getts the code from the Buffer.
/// </summary>
/// <param name="buffer"> The buffer with the code.</param>
/// <returns> The code in the head of the buffer.</returns>
int Communicator::getCode(const Buffer& buffer)
{
	string code = "";
	for (int i = 0; i < SIZE_CODE_FIELD; i++)
	{
		code += buffer[i];
	}
	return atoi(code.c_str());
}

