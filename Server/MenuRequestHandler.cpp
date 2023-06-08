#include "MenuRequestHandler.h"
#include "RoomAdminRequestHandler.h"

/// <summary>
/// C'tor of class MenuRequestHandler
/// </summary>
/// <param name="loggedUser"> The logged user to handle</param>
/// <param name="roomManager"> The room manager for the user</param>
/// <param name="statisticsManager"> The statistics manager for the user</param>
/// <param name="handlerFactory"> The handlerFactory of the object.</param>
MenuRequestHandler::MenuRequestHandler(const LoggedUser& loggedUser, RoomManager& roomManager, StatisticsManager& statisticsManager, RequestHandlerFactory* handlerFactory):
	m_user(loggedUser), m_roomManager(roomManager), m_statisticsManager(statisticsManager), m_handlerFactory(handlerFactory)
{
}

/// <summary>
/// D'tor of class MenuRequestHandler.
/// </summary>
MenuRequestHandler::~MenuRequestHandler()
{
	this->m_handlerFactory = nullptr;
}

/// <summary>
/// THe function checks of the current request is relevent or not.
/// </summary>
/// <param name="requestInfo"> The request to check</param>
/// <returns>If it is relevent or not.</returns>
bool MenuRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    RequestId code = requestInfo.id;
	return LOGOUT_REQS_CODE == code ||
		GET_ROOMS_REQS_CODE == code ||
		GET_PLAYERS_IN_ROOM_REQS_CODE == code ||
		JOIN_ROOM_REQS_CODE == code ||
		CREATE_ROOM_REQS_CODE == code ||
		GET_HIGH_SCORE_REQS_CODE == code ||
		GET_PERS_STATS_REQS_CODE == code;
}

/// <summary>
/// The function handles the request according to its
/// info.
/// </summary>
/// <param name="requestInfo"> The info about the request.</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
	RequestResult* res = new RequestResult();

	if (!this->isRequestRelevent(requestInfo))
	{
		createErrorResponse(ERROR_MSG, res);
	}
	else
	{
		switch (requestInfo.id)
		{
		case LOGOUT_REQS_CODE:
			*res = signout(requestInfo);
			break;
		case GET_ROOMS_REQS_CODE:
			*res = getRooms(requestInfo);
			break;
		case GET_PLAYERS_IN_ROOM_REQS_CODE:
			*res = getPlayersInRoom(requestInfo);
			break;
		case JOIN_ROOM_REQS_CODE:
			*res = joinRoom(requestInfo);
			break;
		case CREATE_ROOM_REQS_CODE:
			*res = createRoom(requestInfo);
			break;
		case GET_HIGH_SCORE_REQS_CODE:
			*res =  getHighScore(requestInfo);
			break;
		case GET_PERS_STATS_REQS_CODE:
			*res =  getPersonalStats(requestInfo);
			break;
		}
	}

    return *res;
}

/// <summary>
/// The function creates an ErrorResponse struct and saves it in
/// the RequestResult variable.
/// </summary>
/// <param name="errMsg"> The message of the error.</param>
/// <param name="reqRes"> The RequestResult to put  the ErrorResponse in.</param>
void MenuRequestHandler::createErrorResponse(const string errMsg, RequestResult* reqRes)
{
	ErrorResopnse errResp;
	errResp.message = errMsg;
	reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);//turn the error message into buffer
	reqRes->newHandler = this;//if there is a error the new handler will be the current
}

/// <summary>
/// The function signs out the user from the application
/// </summary>
/// <param name="requestInfo"> The info about this request.</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::signout(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	LogoutResponse logOutResp = LogoutResponse();

	//Logging out
	this->m_handlerFactory->getLoginManager()->logOut(this->m_user.getUsername());

	//Making the LogoutResponse
	logOutResp.status = OK_STATUS_CODE;

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(logOutResp);
	req->newHandler = this->m_handlerFactory->createLoginRequestHandler();
	
	return *req;
}

/// <summary>
/// The function getts all the active rooms.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::getRooms(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	GetRoomsResponse getRoomsResp = GetRoomsResponse();

	//Making the GetRoomsResponse
	getRoomsResp.status = OK_STATUS_CODE;
	getRoomsResp.rooms = this->m_roomManager.getRooms();

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(getRoomsResp);
	req->newHandler = this;

	getRoomsResp.rooms.~vector();
	return *req;
}

/// <summary>
/// The function getts all the player in 
/// a specific room.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::getPlayersInRoom(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	GetPlayersInRoomRequest& getPlayersReqs = JsonRequestPacketDeserializer::desrializeGetPlayersRequest(requestInfo.buffer);
	GetPlayersInRoomResponse getPlayersResp = GetPlayersInRoomResponse();

	//Making the GetPlayersInRoomResponse
	getPlayersResp.players = this->m_roomManager.getRoom(getPlayersReqs.roomId).getAllUsers();

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(getPlayersResp);
	req->newHandler = this;

	getPlayersResp.players.~vector();
	delete &getPlayersReqs;
	return *req;
}

/// <summary>
/// The function getts the personal statistics of
/// the logged user.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::getPersonalStats(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	GetPersonalStatsResponse getPersStatsResp = GetPersonalStatsResponse();

	//Making the GetPersonalStatsResponse
	getPersStatsResp.status = OK_STATUS_CODE;
	getPersStatsResp.statistics = this->m_statisticsManager.getUserStatistics(this->m_user.getUsername());

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(getPersStatsResp);
	req->newHandler = this;

	getPersStatsResp.statistics.~vector();
	return *req;
}

/// <summary>
/// The function getts the top high scores from 
/// all the records ever.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::getHighScore(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	GetHighScoreResponse getHighScoreResp = GetHighScoreResponse();

	//Making the GetHighScoreResponse
	getHighScoreResp.status = OK_STATUS_CODE;
	getHighScoreResp.statistics = this->m_statisticsManager.getHighScore();

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(getHighScoreResp);
	req->newHandler = this;

	getHighScoreResp.statistics.~vector();
	return *req;
}

/// <summary>
/// The function joins the user to a room.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::joinRoom(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	JoinRoomRequest& joinRoomReqs = JsonRequestPacketDeserializer::desrializeJoinRoomRequest(requestInfo.buffer);
	JoinRoomResponse joinRoomResp = JoinRoomResponse();

	try
	{
		this->m_roomManager.getRoom(joinRoomReqs.roomId).addUser(this->m_user);
	}
	catch (const std::exception& excp)
	{
		createErrorResponse(excp.what(), req);
		delete& joinRoomReqs;
		return *req;
	}
	
	//Making the JoinRoomResponse
	joinRoomResp.status = OK_STATUS_CODE;
	
	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(joinRoomResp);
	req->newHandler = this->m_handlerFactory->createRoomMemberRequestHandler(this->m_user, this->m_roomManager.getRoom(joinRoomReqs.roomId));

	delete& joinRoomReqs;
	return *req;
}

/// <summary>
/// The function creates a room.
/// </summary>
/// <param name="requestInfo"> The info about the request</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::createRoom(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	CreateRoomRequest& createRoomReqs = JsonRequestPacketDeserializer::desrializeCreateRoomRequest(requestInfo.buffer);
	CreateRoomResponse createRoomResp = CreateRoomResponse();

	//Creating the room
	RoomData roomData;
	roomData.id = static_cast<int>(this->m_roomManager.getRooms().size() + 1);
	roomData.name = createRoomReqs.roomName;
	roomData.isActive = false;
	roomData.maxPlayers = createRoomReqs.maxUsers;
	roomData.numOfQuestionsInGame = createRoomReqs.questionCount;
	roomData.timePerQuestion = createRoomReqs.answerTimeout;
	this->m_roomManager.createRoom(this->m_user, roomData);

	//Making the CreateRoomResponse
	createRoomResp.status = OK_STATUS_CODE;

	//Making the RequestResult
	req->response = JsonResponsePacketSerializer::serializeResponse(createRoomResp);
	req->newHandler = this->m_handlerFactory->createRoomAdminRequestHandler(this->m_user, Room(roomData, this->m_user));

	delete& createRoomReqs;
	return *req;
}
