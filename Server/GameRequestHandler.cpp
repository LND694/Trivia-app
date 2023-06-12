#include "GameRequestHandler.h"

/// <summary>
/// C'tor of class GameRequestHandler.
/// </summary>
/// <param name="game"> The game</param>
/// <param name="loggedUser"> The user who is in the game</param>
/// <param name="gameManager"> The manager of all games</param>
/// <param name="reqHandlerFactory"> The factory of handlers.</param>
GameRequestHandler::GameRequestHandler(Game game, LoggedUser loggedUser, GameManager& gameManager, RequestHandlerFactory* reqHandlerFactory):
    m_game(game), m_loggedUser(loggedUser), m_gameManager(gameManager), m_requestHandlerFactory(reqHandlerFactory)
{
}

bool GameRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    RequestId id = requestInfo.id;
    return LEAVE_GAME_REQS_CODE == id || GET_QUESTION_REQS_CODE == id ||
        SUBMIT_ANSWER_REQS_CODE == id || GET_GAME_RESULT_REQS_CODE == id;
}

RequestResult& GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();

    if (!isRequestRelevent(requestInfo))
    {
        createErrorResponse(ERROR_MSG, reqRes);
        return *reqRes;
    }
    delete reqRes;
    switch (requestInfo.id)
    {
    case LEAVE_GAME_REQS_CODE:
        *reqRes = leaveGame(requestInfo);
        break;
    case GET_QUESTION_REQS_CODE:
        *reqRes = getQuestion(requestInfo);
        break;
    case SUBMIT_ANSWER_REQS_CODE:
        *reqRes = submitAnswer(requestInfo);
        break;
    case GET_GAME_RESULT_REQS_CODE:
        *reqRes = getGameResults(requestInfo);
        break;
    }
    return *reqRes;
}

RequestResult& GameRequestHandler::getQuestion(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetQuestionResponse resp = GetQuestionResponse();
    Question& question = this->m_game.getQuestionForUser(this->m_loggedUser);
    vector<string>& theAnswers = question.getAnswers();
    unsigned int id = 0;

    //Checking if there is a chance that the user finished to answer all the questions he needs

    //Preparing the GetQuestionResponse for serialization
    resp.status = OK_STATUS_CODE;
    resp.question = question.getQuestion();
    resp.answers = map<unsigned int, string>();

    //Getting the answers of the question
    for (auto i = theAnswers.begin(); i != theAnswers.end(); i++)
    {
        resp.answers.insert({ id, *i });
        id++;
    }

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this;

    delete& question;
    delete& theAnswers;

    return *reqRes;
}

RequestResult& GameRequestHandler::submitAnswer(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    SubmitAnswerResponse resp = SubmitAnswerResponse();
    SubmitAnswerRequest& reqs = JsonRequestPacketDeserializer::desrializeSubmitAnswerRequest(reqInfo.buffer);

    //Submitting the answer
    //Should be something here

    resp.status = OK_STATUS_CODE;
    //resp.correctAnswerId = ?

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this;

    delete& reqs;

    return *reqRes;
}

RequestResult& GameRequestHandler::getGameResults(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetGameResultsResponse resp = GetGameResultsResponse();

    //Checking if the game is over or not

    //Getting the results of the game

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    //reqRes->newHandler = this//MenuRequestHandler;

    return *reqRes;
}

RequestResult& GameRequestHandler::leaveGame(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    LeaveGameResponse resp = LeaveGameResponse();

    //Leaving the game
    this->m_game.removePlayer(this->m_loggedUser);

    //Preparing the LeaveGameResponse for serialization
    resp.status = OK_STATUS_CODE;

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this;

    return *reqRes;
}

/// <summary>
/// The function creates an ErrorResponse struct and saves it in
/// the RequestResult variable.
/// </summary>
/// <param name="errMsg"> The message of the error.</param>
/// <param name="reqRes"> The RequestResult to put  the ErrorResponse in.</param>
void GameRequestHandler::createErrorResponse(const string errMsg, RequestResult* reqRes)
{
    ErrorResopnse errResp;
    errResp.message = errMsg;
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(errResp);//turn the error message into buffer
    reqRes->newHandler = this;//if there is a error the new handler will be the current
}
