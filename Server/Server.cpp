#include "Server.h"
/// <summary>
/// C'tor of class Server
/// </summary>
Server::Server():
	m_communicator()
{

}

/// <summary>
/// D'tor of class Server
/// </summary>
Server::~Server()
{

}

/// <summary>
/// The function activates the server
/// and it starts to listening for connections
/// </summary>
void Server::run()
{
	std::thread comThread(&Communicator::startHandleRequests, this->m_communicator);
	std::string input = "";

	comThread.detach();

	//Waiting to input untill asking for exit
	while (input != EXIT_MSG)
	{
		std::cin >> input;
	}
}
