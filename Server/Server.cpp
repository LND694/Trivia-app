#include "Server.h"


/// <summary>
/// C'tor of class Server
/// </summary>
/// <param name="db"> The database of the server</param>
Server::Server(IDatabase* db):
	m_database(db), m_handlerFactory(db, LoginManager(db)), m_communicator(m_handlerFactory)
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

	std::cout << "Write '"<< EXIT_MSG << "' to power off the server" << std::endl;
	//Waiting to input until asking for exit
	while (input != EXIT_MSG)
	{
		std::cin >> input;
	}
}
