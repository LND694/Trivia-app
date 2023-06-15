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

/// <summary>
/// D'tor of class MongoDatabase.
/// </summary>
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
		//create collections if not exists
		if (!this->db.has_collection(USERS_COLLECTION))
		{
			this->db.create_collection(USERS_COLLECTION);
		}
		if (!this->db.has_collection(STATS_COLLECTION))
		{
			this->db.create_collection(STATS_COLLECTION);
		}
		if (!this->db.has_collection(QUESTIONS_COLLECTION))
		{
			this->db.create_collection(QUESTIONS_COLLECTION);
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
/// <param username="username">the username to check for</param>
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
	auto doc2 = make_document(kvp("average_time", 0.0),
		kvp("correct_answers", 0),
		kvp("total_answers", 0),
		kvp("total_games", 0),
		kvp("score", 0),
		kvp("username",user.getUsername()));
	try
	{
		this->db[USERS_COLLECTION].insert_one(doc.view());
		this->db[STATS_COLLECTION].insert_one(doc2.view());
		return OK_CODE;
	}
	catch (...)
	{
		return ERROR_CODE;
	}
}

/// <summary>
/// gets all the questions from the database
/// </summary>
/// <param username="amountQuestions"> the amount of questions to get</param>
/// <returns> list of questions</returns>
list<Question>& MongoDatabase::getQuestions(const int amountQuestions)
{
	list<Question>* questions = new list<Question>();
	string question;
	string category;
	string correctAnswer;
	string difficulty;
	string response;
	vector<string> incorrectAnswers;
	// Find all documents in the collection
	auto cursor = this->db[QUESTIONS_COLLECTION].find({});

	for (auto&& doc : cursor)
	{
		category = doc["category"].get_string();
		question = doc["question"].get_string();
		correctAnswer = doc["correct_answer"].get_string();
		difficulty = doc["difficulty"].get_string();
		for (auto&& incorrectAnswer : doc["incorrect_answers"].get_array().value) {
			incorrectAnswers.push_back(incorrectAnswer.get_string().value.data());//get_string() is not working
		}
		questions->push_back(Question(question, incorrectAnswers, correctAnswer, category, difficulty));
		incorrectAnswers.clear();//reset incorrect answers
	}
	return *questions;
}

/// <summary>
/// gets player vaerage answer time
/// </summary>
/// <param username="player"> the username of the player to get</param>
/// <returns> the average answer time</returns>
float MongoDatabase::getPlayerAverageAnswerTime(const string player)
{
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << player 
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];
	// Find the document matching the filter
	auto result = coll.find_one(filter.view());

	// Extract the average time from the resulting document
	if (result) {
		auto docView = result->view();
		auto avgTime = docView["average_time"].get_double();

		return static_cast<float>(avgTime);
	}
	throw std::exception("No document found for the given username");
	
}
/// <summary>
/// get the amount of correct answers of a player
/// </summary>
/// <param username="player"> the username of the player</param>
/// <returns> the amount of correct answers</returns>
int MongoDatabase::getNumOfCorrectAnswers(const string player)
{
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << player
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];
	// Find the document matching the filter
	auto result = coll.find_one(filter.view());

	// Extract the amount of correct answers from the resulting document
	if (result) {
		auto docView = result->view();
		auto correctNum = docView["correct_answers"].get_int32();
		return static_cast<int>(correctNum);
	}
	throw std::exception("No document found for the given username");
}

/// <summary>
/// gets the total amount of answers of the player
/// </summary>
/// <param username="player"> the username of the player</param>
/// <returns> the total amount of answers</returns>
int MongoDatabase::getNumOfTotalAnswers(const string player)
{
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << player
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];
	// Find the document matching the filter
	auto result = coll.find_one(filter.view());

	// Extract the amount of total answers from the resulting document
	if (result) {
		auto docView = result->view();
		auto answersNum = docView["total_answers"].get_int32();
		return static_cast<int>(answersNum);
	}
	throw std::exception("No document found for the given username");
}


/// <summary>
/// gets the amount of games the player played
/// </summary>
/// <param username="player"> the username of the player</param>
/// <returns> amount of games played</returns>
int MongoDatabase::getNumOfPlayerGames(const string player)
{
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << player
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];
	// Find the document matching the filter
	auto result = coll.find_one(filter.view());

	// Extract the amount of total games from the resulting document
	if (result) {
		auto docView = result->view();
		auto answersNum = docView["total_games"].get_int32();
		return static_cast<int>(answersNum);
	}
	throw std::exception("No document found for the given username");
}
/// <summary>
/// gets the score of a player
/// </summary>
/// <param username="player"> the username of the player</param>
/// <returns> score of a player</returns>
int MongoDatabase::getPlayerScore(const string player)
{
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << player
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];
	// Find the document matching the filter
	auto result = coll.find_one(filter.view());

	// Extract the score from the resulting document
	if (result) {
		auto docView = result->view();
		auto score = docView["score"].get_int32();
		return static_cast<int>(score);
	}
	throw std::exception("No document found for the given username");
}

/// <summary>
/// gets the best 5 scores from the db
/// </summary>
/// <returns> vector of scores</returns>
vector<string>& MongoDatabase::getHighScores()
{
	vector<string>* vec = new vector<string>();
	string username;
	string label;//the username of the user with the score
	// Access the STATS_COLLECTION collection
	auto coll = db[STATS_COLLECTION];

	// Aggregate pipeline
	mongocxx::pipeline pipeline{};


	// Sort stage to sort the scores in descending order
	pipeline.sort(bsoncxx::builder::stream::document{} << "score" << -1 << bsoncxx::builder::stream::finalize);

	// Limit stage to get only the top 5 scores
	pipeline.limit(3);

	// Perform the aggregation
	auto cursor = coll.aggregate(pipeline);
	for (auto&& doc : cursor)
	{
		username = doc["username"].get_string().value.data();
		label = std::to_string(doc["score"].get_int32());
		vec->push_back('"'+username + '"'+"," + label);
	}
	return *vec;
}

int MongoDatabase::createGame(const Room& room)
{
	const string COLLECTION_NAME = "GAME_" + room.getRoomData().id;
	vector<string> usersRoom = room.getAllUsers();
	this->db.create_collection(COLLECTION_NAME);

	//Going over the players in the game
	for (auto i = usersRoom.begin(); i != usersRoom.end(); i++)
	{
		auto doc = make_document(kvp("username", *i),
			kvp("current_question", ""), 
			kvp("correct_answers", 0),
			kvp("wrong_answers", 0),
			kvp("average_time", 0.0));
		
		try
		{
			this->db[COLLECTION_NAME].insert_one(doc.view());
		}
		catch (...)
		{
			return ERROR_CODE;
		}
	}
	return OK_CODE;

}

int MongoDatabase::deleteGame(const GameId idGame)
{
	try
	{
		this->db.collection("GAME_" + idGame).drop();
	}
	catch (...)
	{
		return ERROR_CODE;
	}
	return OK_CODE;
}

int MongoDatabase::submitGameStatistics(const GameData& gameData, const LoggedUser userData)
{
	string username = userData.getUsername();
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << username
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];

	StatisticsUser statistics = StatisticsUser(-1, 
		this->getNumOfPlayerGames(username), 
		this->getNumOfTotalAnswers(username), 
		this->getNumOfCorrectAnswers(username), 
		this->getPlayerAverageAnswerTime(username), 
		this->getPlayerScore(username));

	IDatabase::updateStatistics(statistics, gameData);

	auto result = coll.update_one(filter.view(), make_document(kvp("$set",
		make_document(kvp("average_time", statistics.getAverageAnswerTime()), 
			kvp("correct_answers", statistics.getAmountCorrectAnswers()), 
			kvp("total_answers", statistics.getAmountTotalAnswers()), 
			kvp("total_games", statistics.getAmountTotalAnswers()), 
			kvp("score", statistics.getHighScore())
		))));
	if (!result)
	{
		return ERROR_CODE;
	}

	return OK_CODE;
}

/// <summary>
/// insert questions to the mongo database
/// </summary>
/// <param username="numOfQuestions"> the number of questions to insert</param>
void MongoDatabase::insertQuestions(const int numOfQuestions)
{
	vector<Question> questions = fetchQuestions(numOfQuestions);//get questions from opentdb api

	if (!this->db.has_collection(QUESTIONS_COLLECTION))
	{
		this->db.create_collection(QUESTIONS_COLLECTION);
	}
	auto coll = this->db[QUESTIONS_COLLECTION];
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



