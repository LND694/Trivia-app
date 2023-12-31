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
	insertQuestions(DEFAULT_QUESTIONS_AMOUNT);
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

/// <summary>
/// The function checks if a certain user has a 
/// specific password.
/// </summary>
/// <param name="username"> The username of the User.</param>
/// <param name="password"> The password to check.</param>
/// <returns>an integer variable- if the password matches, does not matches or if there was an error.</returns>
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

/// <summary>
/// The function adds another user to the database.
/// </summary>
/// <param name="user">The user to insert</param>
/// <returns>an integer value- if the user already exsit, if there was
/// an error or if the adding was OK.</returns>
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
	vector<string> answers;
	// Create the aggregation pipeline
	mongocxx::pipeline pipeline{};
	pipeline.sample(amountQuestions); // Set the number of random documents to retrieve
	// Find all documents in the collection
	auto cursor = this->db[QUESTIONS_COLLECTION].aggregate(pipeline);
	//Going over the Question Documents
	for (auto&& doc : cursor)
	{
		category = doc["category"].get_string();
		question = doc["question"].get_string();
		correctAnswer = doc["correct_answer"].get_string();
		difficulty = doc["difficulty"].get_string();
		for (auto&& incorrectAnswer : doc["incorrect_answers"].get_array().value) {
			answers.push_back(incorrectAnswer.get_string().value.data());//get_string() is not working
		}
		answers.push_back(correctAnswer);
		questions->push_back(Question(question, IDatabase::randomizeOrderAnswers(answers), correctAnswer, category, difficulty));
		answers.clear();//reset incorrect answers
	}
	return *questions;
}

int MongoDatabase::insertQuestionToDB(const Question& question)
{
	// Document with username, password, and email fields
	auto document = bsoncxx::builder::stream::document{};
	auto arrayBuilder = document << "category" << question.getCategory()
		<< "question" << question.getQuestion()
		<< "correct_answer" << question.getRightAnswer()
		<< "difficulty" << question.getDifficulty()
		<< "incorrect_answers" << bsoncxx::builder::stream::open_array;

	for (const auto& incorrectAnswer : question.getAnswers()) {
		arrayBuilder << incorrectAnswer;
	}

	arrayBuilder << bsoncxx::builder::stream::close_array
		<< "fromUser" << question.getIfFromUser();

	try
	{
		this->db[QUESTIONS_COLLECTION].insert_one(document.view());
		return OK_CODE;
	}
	catch (...)
	{
		return ERROR_CODE;
	}
}

/// <summary>
/// gets player average answer time
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

/// <summary>
/// The function submitts the new statistics of the 
/// last game in the old statistics of the user.
/// </summary>
/// <param name="gameData"> The data of the new game.</param>
/// <param name="userData"> The user which this data belongs to him.</param>
/// <returns> If the submittion succeeded or not.</returns>
int MongoDatabase::submitGameStatistics(const GameData& gameData, const LoggedUser& userData)
{
	string username = userData.getUsername();
	// Define the filter to match the desired username
	auto filter = bsoncxx::builder::stream::document{}
		<< "username" << username
		<< bsoncxx::builder::stream::finalize;
	auto coll = this->db[STATS_COLLECTION];

	//Getting the old statistics
	StatisticsUser statistics = StatisticsUser(-1, 
		this->getNumOfPlayerGames(username), 
		this->getNumOfTotalAnswers(username), 
		this->getNumOfCorrectAnswers(username), 
		this->getPlayerAverageAnswerTime(username), 
		this->getPlayerScore(username));

	IDatabase::updateStatistics(statistics, gameData);

	//Updating the document in the database
	auto result = coll.update_one(filter.view(), make_document(kvp("$set",
		make_document(kvp("average_time", statistics.getAverageAnswerTime()), 
			kvp("correct_answers", statistics.getAmountCorrectAnswers()), 
			kvp("total_answers", statistics.getAmountTotalAnswers()), 
			kvp("total_games", statistics.getAmountTotalAnswers()), 
			kvp("score", statistics.getHighScore())
		))));

	if (!result) //the updating failed
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


	// Delete all prev documents in the collection which are not from the users
	auto delete_many_result =
		coll.delete_many(make_document(kvp("fromUser", false)));


	// Insert the questions into MongoDB
	for (const auto& question : questions) {
		insertQuestionToDB(question);
	}
}



