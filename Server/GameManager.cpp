#include "GameManager.h"
#include "Game.h"

/// <summary>
/// The function finds the index of a Game in the 
/// map by its GameId.
/// </summary>
/// <param name="gameId"> The id of the game.</param>
/// <returns> Its index in the map.</returns>
int GameManager::getIndexGame(const GameId gameId) const
{
    int index = 0;

    //Going over the games
    for (auto i = this->m_games.begin(); i != this->m_games.end(); i++)
    {
        if (i->getGameId() == gameId) //the game was found
        {
            return index;
        }
        index++;
    }
    return ERROR_CODE;
}

/// <summary>
/// C'tor of class GameManager.
/// </summary>
/// <param name="db"> The database for the GameManager.</param>
GameManager::GameManager(IDatabase* db):
    m_database(db), m_games(vector<Game>())
{
}

/// <summary>
/// The fucntion creates a Game by the 
/// data of the Room.
/// </summary>
/// <param name="room"> The room which wants to create the game.</param>
/// <returns> The game which was created.</returns>
Game& GameManager::createGame(const Room& room)
{
    int gameIndex = getIndexGame(room.getRoomData().id);
    Game* game = nullptr;
    if (gameIndex != ERROR_CODE) //the game already exists
    {
        return this->m_games[gameIndex];
    }
    //Creating another game.
    game = new Game(room.getRoomData().id, room, *this);
    this->m_games.push_back(*game);
    return *game;
}

/// <summary>
/// The function deletes the game from the map of the games
/// by its GameId.
/// </summary>
/// <param name="gameId"> The id of the game.</param>
void GameManager::deleteGame(const GameId gameId)
{
    std::erase_if(this->m_games, [gameId](Game current) {return current.getGameId() == gameId; });
}

/// <summary>
/// The function submitts the statistics of the user according to 
/// his GameData.
/// </summary>
/// <param name="gameData"> The GameData of the user.</param>
/// <param name="userData"> The data whose the user.</param>
void GameManager::submitStatistics(const GameData& gameData, const LoggedUser& userData)
{
    if(this->m_database->submitGameStatistics(gameData, userData) == ERROR_CODE)
    {
        throw std::exception("Can not update the statistics of the user");
    }
}

/// <summary>
/// The function getts questions from the database.
/// </summary>
/// <param name="amountQuestions"> The amount of questions to get
/// from the database.</param>
/// <returns> The questions from the database.</returns>
vector<Question>& GameManager::getQuestionsFromDB(const int amountQuestions)
{
    list<Question> questions = this->m_database->getQuestions(amountQuestions);
    vector<Question>* theQuestions = new vector<Question>();
    //Copying the list to a vector
    for (auto i = questions.begin(); i != questions.end(); i++)
    {
        theQuestions->push_back(*i);
    }
    return *theQuestions;
}
