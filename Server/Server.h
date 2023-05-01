#pragma once

#include "Communicator.h"

class Server
{
private:
	//Fields
	Communicator m_communicator;

public:
	//C'tor&D'tor
	Server();
	~Server();

	void run();
};