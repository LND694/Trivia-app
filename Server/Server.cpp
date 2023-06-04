#include "Server.h"

Server* Server::m_instance = nullptr;
Lock Server::m_lock;

/// <summary>
/// C'tor of class Server
/// </summary>
/// <param name="db"> The database of the server</param>
Server::Server(IDatabase* db):
	m_database(db)
{
	//this->m_handlerFactory = RequestHandlerFactory::getInstance()
	this->m_handlerFactory = RequestHandlerFactory::getInstance(db, LoginManager::getInstance(db), new RoomManager(), new StatisticsManager(db));
	this->m_communicator = Communicator::getInstance(m_handlerFactory);
}

/// <summary>
/// D'tor of class Server
/// </summary>
Server::~Server()
{
	delete m_communicator;
}

/// <summary>
/// The function getts the instance of the class Server.
/// </summary>
/// <param name="db"> The database of the server.</param>
/// <returns> The only instance of the class Server.</returns>
Server* Server::getInstance(IDatabase* db)
{
	lock_guard<Lock> lockGuard(m_lock);
	if (m_instance == nullptr)
	{
		m_instance = new Server(db);
	}
	return m_instance;
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

	std::cout << "Write '"<< EXIT_MSG << "' to power off the server " << std::endl;
	//Waiting to input until asking for exit
	while (input != EXIT_MSG)
	{
		std::cin >> input;
	}
}
