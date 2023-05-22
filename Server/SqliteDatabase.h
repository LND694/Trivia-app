#pragma once
#include "IDatabase.h"
#include "User.h"
#include "sqlite3.h"
#include <list>


using std::list;



const string DB_FILE_NAME = "triviaDB.sqlite";
const string COMMANDS_CREATING_DB[] = {"CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,\
USERNAME TEXT NOT NULL, \
PASSWORD TEXT NOT NULL, \
EMAIL TEXT NOT NULL, \
ADDRESS TEXT NOT NULL, \
PHONE_NUM TEXT NOT NULL, \
BORN_DATE TEXT NOT NULL);"};

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


	//Question functions
	list<Question>& getQuestions(const int amountQuestions) override;
	float getPlayerAverageAnswerTime(const string player) override;
	int getNumOfCorrectAnswers(const string player) override;
	int getNumOfTotalAnswers(const string player) override;
	int getNumOfPlayerGames(const string player) override;
	int getPlayerScore(const string player) override;
	vector<string>& getHighScores() override;

	void fetchQuestions(const int numOfQuestions);

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
	static int callbackUsers(void* data, int argc, char** argv, char** azColName);
	template <class T>
	list<T>* runSqlCommand(const string command);
};
