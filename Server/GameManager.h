#pragma once
#include "Game.h"
#include "IDatabase.h"

class Game;

class GameManager
{
private:
	//Fields
	IDatabase* m_database;
	vector<Game> m_games;

	//Help function
	int getIndexGame(const GameId gameId) const;
public:
	Game& createGame(const Room& room);
	void deleteGame(const GameId gameId);
	void submitStatistics(const GameData& gameData, const GameId idGame, const LoggedUser userData);

	vector<Question>& getQuestionsFromDB(const int amountQuestions);
};