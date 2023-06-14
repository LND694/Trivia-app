#pragma once
#include "IDatabase.h"
#include "User.h"
#include "sqlite3.h"


constexpr int AMOUNT_TABLES = 3;
constexpr int AMOUNT_HIGH_SCORES = 3;
const string DB_FILE_NAME = "triviaDB.sqlite";
const string COMMANDS_CREATING_DB[] = {"CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
USERNAME TEXT NOT NULL, \
PASSWORD TEXT NOT NULL, \
EMAIL TEXT NOT NULL, \
ADDRESS TEXT NOT NULL, \
PHONE_NUM TEXT NOT NULL, \
BORN_DATE TEXT NOT NULL);",

"CREATE TABLE IF NOT EXISTS QUESTIONS(ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, \
QUESTION TEXT NOT NULL, \
CATEGORY TEXT NOT NULL, \
DIFFICULTY TEXT NOT NULL, \
CORRECT_ANSWER TEXT NOT NULL, \
ANSWER1 TEXT NOT NULL, \
ANSWER2 TEXT NOT NULL, \
ANSWER3 TEXT NOT NULL);" ,

"CREATE TABLE IF NOT EXISTS STATISTICS(USER_ID INTEGER NOT NULL, \
HIGH_SCORE INTEGER, \
AMOUNT_CORRECT_ANSWERS INTEGER, \
AVERAGE_ANSWER_TIME FLOAT, \
TOTAL_AMOUNT_ANSWERS INTEGER, \
AMOUNT_GAMES INTEGER, \
FOREIGN KEY (USER_ID) REFERENCES USERS(ID));"
};

class SqliteDatabase : public IDatabase
{
public:
	//D'tor
	~SqliteDatabase();

	static SqliteDatabase* getInstance();
	SqliteDatabase(SqliteDatabase& other) = delete;
	void operator=(const SqliteDatabase& other) = delete;

	//Open&Close functions 
	bool open() override;
	bool close() override;

	//User functions
	int doesUserExist(const string username) override;
	int doesPasswordMatch(const string username, const string password) override;
	int addNewUser(const User& user) override;

	//Question function
	list<Question>& getQuestions(const int amountQuestions) override;

	//Statistics functions
	float getPlayerAverageAnswerTime(const string player) override;
	int getNumOfCorrectAnswers(const string player) override;
	int getNumOfTotalAnswers(const string player) override;
	int getNumOfPlayerGames(const string player) override;
	int getPlayerScore(const string player) override;
	vector<string>& getHighScores() override;

	//Game Functions
	int createGame(const Room& room);
	int deleteGame(const GameId idGame);
	int submitGameStatistics(const GameData& gameData, const GameId idGame, const LoggedUser userData);

protected:
	//C'tor
	SqliteDatabase();

private:
	//Field
	sqlite3* m_db;

	//Singleton fields
	static SqliteDatabase* m_instance;
	static Lock m_lock;

	//Help functions
	void runSqlCommand(const string command);
	static int callbackIntegers(void* data, int argc, char** argv, char** azColName);
	static int callbackUsers(void* data, int argc, char** argv, char** azColName);
	static int callbackQuestions(void* data, int argc, char** argv, char** azColName);
	static int callbackFloat(void* data, int argc, char** argv, char** azColName);
	static int callbackInt(void* data, int argc, char** argv, char** azColName);
	static int callbackString(void* data, int argc, char** argv, char** azColName);
	template <class T>
	list<T>* runSqlCommand(const string command);
	template <class T>
	T runSqlCommandSingleOutput(const string command);
	
};
