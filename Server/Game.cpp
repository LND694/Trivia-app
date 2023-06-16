#include "Game.h"
#include "GameManager.h"

/// <summary>
/// C'tor of class Game.
/// </summary>
/// <param name="gameId"> The id of the game.</param>
/// <param name="room"> The room which the game is in.</param>
/// <param name="questions"> The questions of the game.</param>
Game::Game(const GameId gameId, const Room& room, GameManager& gameManager):
	m_gameId(gameId), m_gameManager(gameManager)
{
	vector<string>& players = room.getAllUsers();
	
	//Going over the players in the room
	for (auto i = players.begin(); i != players.end(); i++)
	{
		this->m_players.insert({ LoggedUser(*i), GameData() });
	}

	this->m_questions = this->m_gameManager.getQuestionsFromDB(room.getRoomData().numOfQuestionsInGame);

	players.~vector();
}

/// <summary>
/// C'tor of class Game.
/// </summary>
/// <param name="gameManager"> THe manager of all games.</param>
Game::Game(GameManager& gameManager):
	m_gameId(-1), m_players(map<LoggedUser, GameData>()), m_questions(vector<Question>()), m_gameManager(gameManager)
{
	
}

/// <summary>
/// D'tor of class Game.
/// </summary>
Game::~Game()
{
	this->m_players.clear();
	this->m_questions.~vector();
}

/// <summary>
/// The function getts a question for the user.
/// </summary>
/// <param name="user"> The user who wants to get the question.</param>
/// <returns> The question for the user.</returns>
Question& Game::getQuestionForUser(const LoggedUser& user) const
{
	//Ranomizing an index for the vector of the Questions.
	random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, static_cast<int>(this->m_questions.size()) - 1);
	int randomIndex = dist(rng);
	Question* question = new Question(this->m_questions[randomIndex]);

	return *question;
}

/// <summary>
/// The function removes the user from the game.
/// </summary>
/// <param name="user"> The user who wants to leave the game.</param>
void Game::removePlayer(const LoggedUser& user)
{
	if (this->m_players.find(user) != this->m_players.end())
	{
		throw std::exception("This user is not in the game");
	}
	this->m_players.erase(user);
}

/// <summary>
/// The function getts the GameData of the user.
/// </summary>
/// <param name="user"> The user who has the GameData.</param>
/// <returns>The GameData for the user.</returns>
GameData& Game::getGameDataOfUser(const LoggedUser& user)
{
	if (this->m_players.find(user) != this->m_players.end())
	{
		throw std::exception("This user is not in the game");
	}
	return this->m_players.at(user);
}

/// <summary>
/// The function checks if all the players has finished their game.
/// </summary>
/// <returns> If the game is over</returns>
bool Game::isGameOver() const
{
	//Going over the players
	for (auto i = this->m_players.begin(); i != this->m_players.end(); i++)
	{
		if (!isUserFinished(i->first))
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// The function checks if a single user has finished the game.
/// </summary>
/// <param name="user"> The user who the check is on him.</param>
/// <returns> If he finished or not.</returns>
bool Game::isUserFinished(const LoggedUser& user) const
{
	unsigned int amountQuestionsInGame = static_cast<unsigned int>(this->m_questions.size());
	GameData dataUser = this->m_players.at(user);
	return dataUser.correctAnswerCount + dataUser.wrongAnswerCount >= amountQuestionsInGame ||
		dataUser.currentQuestion.getQuestion() == "";
}

GameId Game::getGameId() const
{
	return this->m_gameId;
}

vector<LoggedUser>& Game::getGameUsers() const
{
	vector<LoggedUser>* users = new vector<LoggedUser>();

	//Going over the map of the players
	for (auto i = this->m_players.begin(); i != this->m_players.end(); i++)
	{
		users->push_back(i->first);
	}
	return *users;
}

int Game::getAmountQuestionsInGame() const
{
	return static_cast<int>(this->m_questions.size());
}

/// <summary>
/// The operator '=' copying the object Game. 
/// </summary>
/// <param name="other"> The other object to copy.</param>
/// <returns></returns>
Game& Game::operator=(const Game& other) const
{
	Game* game = new Game(other.m_gameManager);

	//Copying the fields of the other game
	game->m_gameId = other.m_gameId;
	game->m_players = map<LoggedUser, GameData>(other.m_players);
	game->m_questions = vector<Question>(other.m_questions);

	return *game;
}
