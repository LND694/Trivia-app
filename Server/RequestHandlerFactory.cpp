#include "RequestHandlerFactory.h"
#include "RoomAdminRequestHandler.h"

RequestHandlerFactory* RequestHandlerFactory::m_instance = nullptr;
Lock RequestHandlerFactory::m_lock;

/// <summary>
/// C'tor of class RequestHandlerFactory
/// </summary>
/// <param name="db"> The database of the factory</param>
/// <param name="logMan"> The manager of the logins</param>
/// <param name="roomMan"> The manager of the rooms</param>
/// <param name="statisticsMan"> The manager of the statistics</param>
RequestHandlerFactory::RequestHandlerFactory(IDatabase* db, LoginManager* logMan,
	RoomManager* roomMan, StatisticsManager* statisticsMan) :
    m_dataBase(db), m_loginManager(logMan), m_roomManager(roomMan), m_statisticsManager(statisticsMan)
{
}

/// <summary>
/// The function getts the instance of the object RequestFactoryHandler.
/// </summary>
/// <param name="db"> The database for the RequestHandlerFactory.</param>
/// <param name="logMan"> The login manager for the RequestHandlerFactory.</param>
/// <returns> THe instance of the RequestHandlerFactory class.</returns>
RequestHandlerFactory* RequestHandlerFactory::getInstance(IDatabase* db, LoginManager* logMan,
	RoomManager* roomMan, StatisticsManager* statisticsMan)
{
	lock_guard<Lock> lockGuard(m_lock);
	if (m_instance == nullptr)
	{
		m_instance = new RequestHandlerFactory(db, logMan, roomMan, statisticsMan);
	}
	return m_instance;
}

/// <summary>
/// The function creates a handler to login requests.
/// </summary>
/// <returns>a LoginRequestHandler value- the handler of the login requests.</returns>
LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler(*this);
}

/// <summary>
/// The function creates a MenuRequestHandler.
/// </summary>
/// <param name="user"> The logged user to handle</param>
/// <returns>The handler for the menu stage.</returns>
MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser user)
{
	return new MenuRequestHandler(user, *this->m_roomManager, *this->m_statisticsManager, this);
}

/// <summary>
/// The function creates a RoomMemberRequestHandler.
/// </summary>
/// <param name="user"> The member which wants to enter to the room</param>
/// <param name="room"> The room which the member enters to.</param>
/// <returns> The handler for the member.</returns>
RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser user, const Room room)
{
	return new RoomMemberRequestHandler(room, user, *this->m_roomManager, this);
}

/// <summary>
/// The function creates a RoomAdminRequestHandler.
/// </summary>
/// <param name="user"> The admin which wants to create the room</param>
/// <param name="room"> The room which the admin created.</param>
/// <returns> The handler for the admin.</returns>
RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser user, const Room room)
{
	return new RoomAdminRequestHandler(room, user, *this->m_roomManager, this);
}

/// <summary>
/// The function getts the login manager of the factory.
/// </summary>
/// <returns>a reference to LoginManager variable- the login manager.</returns>
LoginManager* RequestHandlerFactory::getLoginManager()
{
    return this->m_loginManager;
}

/// <summary>
/// The function getts the room manager of the factory.
/// </summary>
/// <returns>a reference to RoomManager variable- the room manager.</returns>
RoomManager& RequestHandlerFactory::getRoomManager()
{
	return *this->m_roomManager;
}

/// <summary>
/// The function getts the statistics manager of the factory.
/// </summary>
/// <returns>a reference to StatisticsManager variable- the login manager.</returns>
StatisticsManager& RequestHandlerFactory::getStatisticsManager()
{
	return *this->m_statisticsManager;
}
