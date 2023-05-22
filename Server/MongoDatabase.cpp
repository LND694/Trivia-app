#include "MongoDatabase.h"

/// <summary>
/// c`tor for mongo database
/// </summary>
MongoDatabase::MongoDatabase()
{
	if (!this->open())
	{
		throw std::exception("Cannot open the database");
	}
	insertQuestions(DEFAULT_QUESTIONS_AMOUNT);//insert 10 question everytime
}

MongoDatabase::~MongoDatabase()
{
}

/// <summary>
/// open mongo database
/// </summary>
/// <returns> if the database was opened ssucessfully or not</returns>
bool MongoDatabase::open()
{
	try
	{
		this->client = mongocxx::client{ mongocxx::uri{} };//connect to the server
		auto dbs = this->client.list_database_names();
		
		this->db = this->client[DB_NAME];
		if (!this->db.has_collection(USERS_COLLECTION))
		{
			this->db.create_collection(USERS_COLLECTION);
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}

/// <summary>
/// close the server
/// </summary>
/// <returns>false</returns>
bool MongoDatabase::close()//the database is closed automatically
{
	return false;
}

/// <summary>
/// check if a user exists
/// </summary>
/// <param name="username">the username to check for</param>
/// <returns> true or false</returns>
int MongoDatabase::doesUserExist(const string username)
{
	// Build the query
	auto doc = make_document(kvp(USERNAME_FIELD, username));

	// Execute the query
	auto cursor = this->db[USERS_COLLECTION].find(doc.view());
	if (cursor.begin() != cursor.end())//if there were found
	{
		return USER_EXIST;
	}
	return USER_NOT_EXIST;
}

int MongoDatabase::doesPasswordMatch(const string username, const string password)
{
	auto doc = make_document(kvp(USERNAME_FIELD, username), kvp(PASSWORD_FIELD, password));
	// Execute the query
	auto cursor = this->db[USERS_COLLECTION].find(doc.view());

	if (cursor.begin() != cursor.end())// if there were found
	{
		return PASSWORD_MATCH;
	}
	return PASSWORD_NOT_MATCH;
}

int MongoDatabase::addNewUser(const User& user)
{
	// Document with username, password, and email fields
	auto doc = make_document(kvp(EMAIL_FIELD, user.getEmail()),
		kvp(USERNAME_FIELD, user.getUsername()),
		kvp(PASSWORD_FIELD, user.getPassword()),
		kvp(ADDRESS_FIELD,user.getAddress()),
		kvp(BORN_DATE_FIELD,user.getBornDate()),
		kvp(PHONE_NUM_FIELD,user.getPhoneNum()));
	try
	{
		auto insert_one_result = this->db[USERS_COLLECTION].insert_one(doc.view());
		return OK_CODE;
	}
	catch (...)
	{
		return ERROR_CODE;
	}
}

list<Question>& MongoDatabase::getQuestions(const int amountQuestions)
{
	list<Question>* q = new list<Question>();
	return *q;
}

float MongoDatabase::getPlayerAverageAnswerTime(const string player)
{
	return 0.0f;
}

int MongoDatabase::getNumOfCorrectAnswers(const string player)
{
	return 0;
}

int MongoDatabase::getNumOfTotalAnswers(const string player)
{
	return 0;
}

int MongoDatabase::getNumOfPlayerGames(const string player)
{
	return 0;
}

int MongoDatabase::getPlayerScore(const string player)
{
	return 0;
}

vector<string>& MongoDatabase::getHighScores()
{
	vector<string>* vec = new vector<string>();
	return *vec;
}

/// <summary>
/// insert questions to the mongo database
/// </summary>
/// <param name="numOfQuestions"> the number of questions to insert</param>
void MongoDatabase::insertQuestions(const int numOfQuestions)
{
	vector<Question> questions = fetchQuestions(numOfQuestions);//get questions from opentdb api

	if (!this->db.has_collection("QUESTIONS"))
	{
		this->db.create_collection("QUESTIONS");
	}
	auto coll = this->db["QUESTIONS"];
	// Delete all prev documents in the collection
	coll.delete_many({});
	// Insert the questions into MongoDB
	for (const auto& question : questions) {
		// Create a BSON document with an array
		auto document = bsoncxx::builder::stream::document{};
		auto arrayBuilder = document << "category" << question.getCategory()
			<< "question" << question.getQuestion()
			<< "correct_answer" << question.getRightAnswer()
			<< "difficulty" << question.getDifficulty()
			<< "incorrect_answers" << bsoncxx::builder::stream::open_array;

		for (const auto& incorrectAnswer : question.getAnswers()) {
			arrayBuilder << incorrectAnswer;
		}

		arrayBuilder << bsoncxx::builder::stream::close_array;

		// Insert the document into the collection
		coll.insert_one(document.view());
	}
}



