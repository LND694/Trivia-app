#include "Game.h"

void Game::submitGameStatsToDB(const GameData& gameData)
{

}

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
	submitGameStatsToDB(this->m_players.at(user));

	std::erase_if(this->m_players, [&user](LoggedUser currentUser) {return user.getUsername() == currentUser.getUsername(); });

}

GameId Game::getGameId() const
{
	return this->m_gameId;
}
