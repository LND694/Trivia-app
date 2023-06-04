#include "Server.h"
#include "WSAInitializer.h"
#pragma comment (lib, "Ws2_32.lib")

int main(void)
{
	IDatabase* db = nullptr;
	Server* server = nullptr;
	mongocxx::instance instance{}; // This should be done only once.
	try
	{
		//db = SqliteDatabase::getInstance();
		db = new MongoDatabase();
		WSAInitializer wasinit;//init before init the socket because the sokcet depends on it
		server = Server::getInstance(db);
		server->run();
		

	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << endl;
		return 1;
	}
	delete server;
	server = nullptr;
	db->close();
	delete db;
	db = nullptr;
	return 0;
}