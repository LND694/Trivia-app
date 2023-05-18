#pragma once

#include "global.h"
#include "Singleton.h"
#include "SqliteDatabase.h"
#include "RequestHandlerFactory.h"
#include "Communicator.h"

const std::string EXIT_MSG  = "exit";

class Server : public Singleton
{
private:
	//Singleoton fields
	static Server* m_instance;
	static Lock m_lock;

protected:
	//C'tor
	Server(IDatabase* db);
	//Fields
	Communicator* m_communicator;
	IDatabase* m_database;
	RequestHandlerFactory* m_handlerFactory;

public:
	//D'tor
	~Server();
	//Canceling copy constructor and copy operator
	Server(Server& other) = delete;
	void operator=(const Server& other) = delete;
	static Server* getInstance(IDatabase* db);

	//Function
	void run();
};