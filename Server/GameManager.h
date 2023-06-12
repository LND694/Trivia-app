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

public:
	Game& createGame(const Room& room);
	void deleteGame(const GameId gameId);
	vector<Question>& getQuestionsFromDB(const int amountQuestions);
};