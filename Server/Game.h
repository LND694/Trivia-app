#pragma once

#include "Question.h"
#include "LoggedUser.h"
#include "Room.h"
#include "global.h"
#include <map>
#include <random>

using std::map;
using std::random_device;

typedef struct GameData
{
	Question currentQuestion;
	unsigned int correctAnswerCount{};
	unsigned int wrongAnswerCount{};
	float averageAnswerTime{};
}GameData;

class GameManager;

class Game
{
private:
	//Fields
	vector<Question> m_questions;
	map<LoggedUser, GameData> m_players;
	GameId m_gameId;
	GameManager& m_gameManager;

public:
	//C'tors
	Game(const GameId gameId, const Room& room, GameManager& gameManager);
	Game(GameManager& gameManager);

	//D'tor
	~Game();

	//Functions
	Question& getQuestionForUser(const LoggedUser& user) const;
	void removePlayer(const LoggedUser& user);
	GameData& getGameDataOfUser(const LoggedUser& user);
	bool isGameOver() const;
	bool isUserFinished(const LoggedUser& user) const;
	bool doesAllGotResults() const;

	//Getters
	GameId getGameId() const;
	vector<LoggedUser>& getGameUsers() const;
	int getAmountQuestionsInGame() const;

	//Operators
	Game& operator=(const Game& other) const;
};

