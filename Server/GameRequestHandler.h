#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
#include "GameManager.h"

class RequestHandlerFactory;

class GameRequestHandler : public IRequestHandler
{
public:
	//C'tor
	GameRequestHandler(Game game, LoggedUser loggedUser, GameManager& gameManager, RequestHandlerFactory* reqHandlerFactory);
	//Functions
	bool isRequestRelevent(const RequestInfo& requestInfo) override;
	RequestResult& handleRequest(const RequestInfo& requestInfo) override;

private:
	//Fields
	Game& m_game;
	LoggedUser& m_loggedUser;
	GameManager& m_gameManager;
	RequestHandlerFactory* m_requestHandlerFactory;

	//Help functions
	RequestResult& getQuestion(const RequestInfo& reqInfo);
	RequestResult& submitAnswer(const RequestInfo& reqInfo);
	RequestResult& getGameResults(const RequestInfo& reqInfo);
	RequestResult& leaveGame(const RequestInfo& reqInfo);

	void createErrorResponse(const string errMsg, RequestResult* reqRes);
};