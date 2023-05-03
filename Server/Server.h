#pragma once

#include "Communicator.h"

const std::string EXIT_MSG = "exit";

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