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
		GET_PERS_STATS_REQS_CODE == code ||
		ADD_QUESTION_REQS_CODE == code;
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
		throw std::exception(ERROR_MSG.c_str());
	}
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
		*res = getHighScore(requestInfo);
		break;
	case GET_PERS_STATS_REQS_CODE:
		*res = getPersonalStats(requestInfo);
		break;
	case ADD_QUESTION_REQS_CODE:
		*res = addQuestion(requestInfo);
	}

    return *res;
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

	this->m_roomManager.getRoom(joinRoomReqs.roomId).addUser(this->m_user);
	
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

/// <summary>
/// The function add a question to the DB.
/// </summary>
/// <param name="requestInfo"> THe info about the question to add.</param>
/// <returns> The result of the request</returns>
RequestResult& MenuRequestHandler::addQuestion(const RequestInfo& requestInfo)
{
	RequestResult* req = new RequestResult();
	AddQuestionResponse addQuestResp = AddQuestionResponse();
	AddQuestionRequest& addQuestReq = JsonRequestPacketDeserializer::deserializeAddQuestionRequest(requestInfo.buffer);
	CHECK_QUESTION_RESULTS resultQuestion = QUESTION_IS_LEGAL;

	//Creating the question to insert
	addQuestReq.wrongAnswers.push_back(addQuestReq.rightAnswer);
	Question question = Question(addQuestReq.question, addQuestReq.wrongAnswers,
		addQuestReq.rightAnswer, "Question from user " + this->m_user.getUsername(),
		addQuestReq.difficulty, true);

	//Making sure that the question is valid
	resultQuestion = UserInputChecker::isQuestionValid(question.getQuestion(), question.getDifficulty());
	if (QUESTION_IS_LEGAL != resultQuestion)
	{
		switch (resultQuestion)
		{
		case NO_QUESTION_MARK:
			throw std::exception("There should be a question mark in the end of the question.\n");
			break;
		case INVALID_DIFFICULTY:
			throw std::exception("This difficulty is not recognized.\n");
			break;
		case NO_QUESTION_WORD:
			throw std::exception("There should be a question word in the start of the question like who or when\n");
			break;
		}
	}

	//adding the question
	this->m_statisticsManager.addQuestion(question);

	//Making the RequestResult
	addQuestResp.status = OK_STATUS_CODE;
	req->response = JsonResponsePacketSerializer::serializeResponse(addQuestResp);
	req->newHandler = this;

	return *req;
}
