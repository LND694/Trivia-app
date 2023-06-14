#include "Game.h"

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

Question& Game::getQuestionForUser(const LoggedUser& user) const
{
	random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<int> dist(0, this->m_questions.size() - 1);
	int randomIndex = dist(rng);
	Question* question = new Question(this->m_questions[randomIndex]);

	return *question;
}

void Game::removePlayer(LoggedUser& user)
{
	if (this->m_players.find(user) != this->m_players.end())
	{
		throw std::exception("This user is not in the room");
	}

	std::erase_if(this->m_players, [&user](LoggedUser currentUser) {return user.getUsername() == currentUser.getUsername(); });

}

void Game::changeGameDataOfUser(const LoggedUser user, const GameData newGameData)
{
	this->m_players.at(user) = newGameData;
}

GameData& Game::getGameDataOfUser(const LoggedUser user)
{
	return this->m_players.at(user);
}

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

bool Game::isUserFinished(const LoggedUser user) const
{
	int amountQuestionsInGame = this->m_questions.size();
	GameData dataUser = this->m_players.at(user);
	return dataUser.correctAnswerCount + dataUser.wrongAnswerCount >= amountQuestionsInGame;
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
	return this->m_questions.size();
}
