#include "GameManager.h"

Game& GameManager::createGame(const Room& room)
{
    Game* game = new Game(room.getRoomData().id, room, *this);
    this->m_games.push_back(*game);
    return *game;
}

void GameManager::deleteGame(const GameId gameId)
{
    std::erase_if(this->m_games, [gameId](Game current) {return current.getGameId() == gameId; });
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
