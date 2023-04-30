#include "Communicator.h"
#include "WSAInitializer.h"
#pragma comment (lib, "Ws2_32.lib")
int main(void)
{
	try
	{
		WSAInitializer wasinit;//init before init the socket beacse the sokcet depends on it
		Communicator* cm = new Communicator();
		cm->startHandleRequests();
		delete cm;
		cm = nullptr;
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << endl;
		return 1;
	}
	return 0;
}