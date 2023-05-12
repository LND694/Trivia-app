#pragma once

#include "SqliteDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"

const std::string EXIT_MSG  = "exit";

class Server
{
private:
	//Fields
	Communicator m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory m_handlerFactory;

public:
	//C'tor&D'tor
	Server(IDatabase* db);
	~Server();

	//Function
	void run();
};