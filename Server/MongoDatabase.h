#pragma once
#include "IDatabase.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::make_array;
using std::string;
const string DB_NAME = "triviaDB";
const string DEAFULT_URL = "mongodb://localhost:27017";
const string USERS_COLLECTION = "USERS";
constexpr int DEFAULT_QUESTIONS_AMOUNT = 10;
class MongoDatabase : public IDatabase
{
public:
	MongoDatabase();
	~MongoDatabase();

	//Open&Close functions 
	bool open() override;
	bool close() override;

	//User functions
	int doesUserExist(const string username) override;
	int doesPasswordMatch(const string username, const string password) override;
	int addNewUser(const  User& user) override;


	//Question functions
	list<Question>& getQuestions(const int amountQuestions) override;
	float getPlayerAverageAnswerTime(const string player) override;
	int getNumOfCorrectAnswers(const string player) override;
	int getNumOfTotalAnswers(const string player) override;
	int getNumOfPlayerGames(const string player) override;
	int getPlayerScore(const string player) override;
	vector<string>& getHighScores() override;

private:
	void insertQuestions(const int numOfQuestions);
	mongocxx::database db;
	mongocxx::client client;
};

