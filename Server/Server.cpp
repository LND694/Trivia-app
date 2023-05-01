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
/// and it starts to get messages
/// </summary>
void Server::run()
{
	this->m_communicator.startHandleRequests();
}
