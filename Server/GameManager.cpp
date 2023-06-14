#include "GameManager.h"

int GameManager::getIndexGame(const GameId gameId) const
{
    int index = 0;

    //Going over the games
    for (auto i = this->m_games.begin(); i != this->m_games.end(); i++)
    {
        if (i->getGameId() == gameId)
        {
            return index;
        }
        index++;
    }
    return ERROR_CODE;
}

Game& GameManager::createGame(const Room& room)
{
    int gameIndex = getIndexGame(room.getRoomData().id);
    Game* game = nullptr;
    if (gameIndex != ERROR_CODE) //the game exists
    {
        return this->m_games[gameIndex];
    }
    game = new Game(room.getRoomData().id, room, *this);
    this->m_games.push_back(*game);
    this->m_database->createGame(room);
    return *game;
}

void GameManager::deleteGame(const GameId gameId)
{
    std::erase_if(this->m_games, [gameId](Game current) {return current.getGameId() == gameId; });
    this->m_database->deleteGame(gameId);
}

void GameManager::submitStatistics(const GameData& gameData, const GameId idGame, const LoggedUser userData)
{
    this->m_database->submitGameStatistics(gameData, idGame, userData);
    this->m_games[getIndexGame(idGame)].changeGameDataOfUser(userData, gameData);
}

vector<Question>& GameManager::getQuestionsFromDB(const int amountQuestions)
{
    list<Question> questions = this->m_database->getQuestions(amountQuestions);
    vector<Question>* theQuestions = new vector<Question>();
    //Copying the list to a vector
    for (auto i = questions.begin(); i != questions.end(); i++)
    {
        theQuestions->push_back(*i);
    }
    questions.~list();
    return *theQuestions;
}
