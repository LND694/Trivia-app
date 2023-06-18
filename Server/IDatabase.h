#pragma once
#include <list>
#include "User.h"
#include "Game.h"
#include "Question.h"
#include "sqlite3.h"
#include "global.h"
#include "Singleton.h"
#include "json.hpp"
#include "StatisticsUser.h"
#include "ScoreCalculator.h"
#include <mongocxx/options/create_collection.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

using  NLOHMANN_JSON_NAMESPACE::json;
using std::list;

const string OPENTDB_URL = "https://opentdb.com/api.php?amount=";


constexpr int AMOUNT_QUESTIONS = 50;
const string SUBSTR1 = "&quot;";
const string SUBSTR2 = "&#039;";

enum RETURNED_CODES {
	ERROR_CODE = -1, OK_CODE = 0,
	USER_EXIST = 1, USER_NOT_EXIST = 2,
	PASSWORD_MATCH = 3, PASSWORD_NOT_MATCH = 4
};

class IDatabase : public Singleton
{
public:

	//Open&Close functions 
	virtual bool open() = 0;
	virtual bool close() = 0;

	//User functions
	virtual int doesUserExist(const string username) = 0;
	virtual int doesPasswordMatch(const string username, const string password) = 0;
	virtual int addNewUser(const User& user) = 0;

	//Question functions
	virtual list<Question>& getQuestions(const int amountQuestions) = 0;

	//Statistics functions
	virtual float getPlayerAverageAnswerTime(const string player) = 0;
	virtual int getNumOfCorrectAnswers(const string player) = 0;
	virtual int getNumOfTotalAnswers(const string player) = 0;
	virtual int getNumOfPlayerGames(const string player) = 0;
	virtual int getPlayerScore(const string player) = 0;
	virtual vector<string>& getHighScores() = 0;

	//Game Functions
	virtual int submitGameStatistics(const GameData& gameData, const LoggedUser& userData) = 0;

protected:
	//Help functions to the inheritors of the class
	static vector<Question>& fetchQuestions(const int numOfQuestions);
	static void updateStatistics(StatisticsUser& oldStats, const GameData& newStats);
	static vector<string>& randomizeOrderAnswers(const vector<string>& answers);
private:
	static string eraseSubString(string str, const string substr);
};