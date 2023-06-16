#include "GameRequestHandler.h"

/// <summary>
/// C'tor of class GameRequestHandler.
/// </summary>
/// <param name="game"> The game</param>
/// <param name="loggedUser"> The user who is in the game</param>
/// <param name="gameManager"> The manager of all games</param>
/// <param name="reqHandlerFactory"> The factory of handlers.</param>
GameRequestHandler::GameRequestHandler(Game& game, LoggedUser& loggedUser, GameManager& gameManager, RequestHandlerFactory* reqHandlerFactory):
    m_game(game), m_loggedUser(loggedUser), m_gameManager(gameManager), m_requestHandlerFactory(reqHandlerFactory)
{

}

/// <summary>
/// The function checks if the request is relevent according to
/// its id.
/// </summary>
/// <param name="requestInfo"> The info about the request which has an id.</param>
/// <returns> If it is relevent or not.</returns>
bool GameRequestHandler::isRequestRelevent(const RequestInfo& requestInfo)
{
    RequestId id = requestInfo.id;
    return LEAVE_GAME_REQS_CODE == id || GET_QUESTION_REQS_CODE == id ||
        SUBMIT_ANSWER_REQS_CODE == id || GET_GAME_RESULT_REQS_CODE == id;
}

/// <summary>
/// The function handles the request.
/// </summary>
/// <param name="requestInfo"> The info about the request.</param>
/// <returns> The result of the request.</returns>
RequestResult& GameRequestHandler::handleRequest(const RequestInfo& requestInfo)
{
    RequestResult* reqRes = new RequestResult();

    //If the request is not relevent
    if (!isRequestRelevent(requestInfo))
    {
        createErrorResponse(ERROR_MSG, reqRes);
        return *reqRes;
    }
    switch (requestInfo.id)
    {
    case LEAVE_GAME_REQS_CODE:
        delete reqRes;
        return leaveGame(requestInfo);
        break;
    case GET_QUESTION_REQS_CODE:
        delete reqRes;
        return getQuestion(requestInfo);
        break;
    case SUBMIT_ANSWER_REQS_CODE:
        delete reqRes;
        return submitAnswer(requestInfo);
        break;
    case GET_GAME_RESULT_REQS_CODE:
        delete reqRes;
        return getGameResults(requestInfo);
        break;
    }
    return *reqRes;
}

/// <summary>
/// The function getts the next question for the user.
/// </summary>
/// <param name="reqInfo"> The info about the request.</param>
/// <returns></returns>
RequestResult& GameRequestHandler::getQuestion(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetQuestionResponse resp = GetQuestionResponse();
    Question& question = this->m_game.getQuestionForUser(this->m_loggedUser);
    vector<string>& theAnswers = question.getAnswers();
    unsigned int id = 0;

    //Checking if there is a chance that the user finished to answer all the questions he needs
    GameData& playerGameData = this->m_game.getGameDataOfUser(this->m_loggedUser);

    resp.answers = map<unsigned int, string>();
    resp.question = "";
    resp.status = ERROR_STATUS_CODE;

    //if the user did not finish to answer all the questions
    if (!this->m_game.isUserFinished(this->m_loggedUser))
    {
        //Making the response 
        resp.status = OK_STATUS_CODE;
        resp.question = question.getQuestion();
        playerGameData.currentQuestion = question;
        
        //Getting the answers of the question
        for (auto i = theAnswers.begin(); i != theAnswers.end(); i++)
        {
            resp.answers.insert({ id, *i });
            id++;
        }
    }
    else
    {
        playerGameData.currentQuestion = Question();
    }

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this;

    delete& question;
    delete& theAnswers;

    return *reqRes;
}

/// <summary>
/// The function submitts the answer of the user.
/// </summary>
/// <param name="reqInfo"> The info about this request.</param>
/// <returns> The result of this request.</returns>
RequestResult& GameRequestHandler::submitAnswer(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    SubmitAnswerResponse resp = SubmitAnswerResponse();
    SubmitAnswerRequest& reqs = JsonRequestPacketDeserializer::desrializeSubmitAnswerRequest(reqInfo.buffer);

    //Submitting the answer and updating the current data of the user
    GameData& currentData = this->m_game.getGameDataOfUser(this->m_loggedUser);
    currentData.averageAnswerTime = ScoreClaculator::calculateAverageTime(currentData.correctAnswerCount + currentData.wrongAnswerCount,
        currentData.averageAnswerTime, 1, static_cast<float>(reqInfo.receivalTime));

    //If the user was correct
    if (reqs.answerId == currentData.currentQuestion.getCorrectAnswerId())
    {
        currentData.correctAnswerCount++;
    }
    else
    {
        currentData.wrongAnswerCount++;
    }

    resp.status = OK_STATUS_CODE;
    resp.correctAnswerId = currentData.currentQuestion.getCorrectAnswerId();

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this;

    delete& reqs;

    return *reqRes;
}

/// <summary>
/// The function getts the results of the game.
/// </summary>
/// <param name="reqInfo"> The info about the request.</param>
/// <returns> The result of the request.</returns>
RequestResult& GameRequestHandler::getGameResults(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    GetGameResultsResponse resp = GetGameResultsResponse();
    PlayerResults currentResult = PlayerResults();
    vector<LoggedUser>& listOfPlayers = this->m_game.getGameUsers();
    GameData currentGameData = GameData();

    resp.results = vector<PlayerResults>();
    
    //Checking if the game is over or not
    if (this->m_game.isGameOver()) // game is over
    {
        resp.status = OK_STATUS_CODE;
        //Getting the results of the game

        //Going over the players of the game and getting their data
        for (auto i = listOfPlayers.begin(); i != listOfPlayers.end(); i++)
        {
            currentGameData = this->m_game.getGameDataOfUser(*i);

            //Updating statistics of user
            this->m_gameManager.submitStatistics(currentGameData, *i);

            //Making the current PlayerResults
            currentResult.username = i->getUsername();
            currentResult.correctAnswerCount = currentGameData.correctAnswerCount;
            currentResult.wrongAnswerCount = currentGameData.wrongAnswerCount;
            currentResult.averageAnswerTime = currentGameData.averageAnswerTime;

            resp.results.push_back(currentResult);
            //Leaving the game
            this->m_game.removePlayer(this->m_loggedUser);
        }

        reqRes->newHandler = this->m_requestHandlerFactory->createMenuRequestHandler(this->m_loggedUser);


        
    }
    else //the game is not over
    {
        resp.status = ERROR_STATUS_CODE;
        reqRes->newHandler = this;
    }

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);

    delete& listOfPlayers;

    return *reqRes;
}

/// <summary>
/// The function removes the player from the game. 
/// </summary>
/// <param name="reqInfo"> The info about the request.</param>
/// <returns> The result of the request.</returns>
RequestResult& GameRequestHandler::leaveGame(const RequestInfo& reqInfo)
{
    RequestResult* reqRes = new RequestResult();
    LeaveGameResponse resp = LeaveGameResponse();

    //A sign that the user is not in the game anymore
    this->m_game.getGameDataOfUser(this->m_loggedUser).currentQuestion.setQuestion("");

    //Preparing the LeaveGameResponse for serialization
    resp.status = OK_STATUS_CODE;

    //Creating the RequestResult
    reqRes->response = JsonResponsePacketSerializer::serializeResponse(resp);
    reqRes->newHandler = this->m_requestHandlerFactory->createMenuRequestHandler(this->m_loggedUser);

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
