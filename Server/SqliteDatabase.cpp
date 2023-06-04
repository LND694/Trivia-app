#include "SqliteDatabase.h"
#include "sqlite3.h"

SqliteDatabase* SqliteDatabase::m_instance = nullptr;
Lock SqliteDatabase::m_lock;

/// <summary>
/// C'tor of class SqliteDatabase
/// </summary>
SqliteDatabase::SqliteDatabase()
{
	if (!this->open()) //failed to open database
	{
		throw std::exception("Can not open database");
	}
}

/// <summary>
/// D'tor of class SqliteDatabase
/// </summary>
SqliteDatabase::~SqliteDatabase()
{
	this->close();
}

/// <summary>
/// The function getts the instance of the class SqliteDatabase.
/// </summary>
/// <returns> the address of the instance</returns>
SqliteDatabase* SqliteDatabase::SqliteDatabase::getInstance()
{
	lock_guard<Lock> lockGuard(m_lock);
	if (m_instance == nullptr)
	{
		m_instance = new SqliteDatabase();
	}
	return m_instance;
}

/// <summary>
/// The function opens the database file. 
/// If the file is not exsit it will create one with the tables.
/// </summary>
/// <returns>a bool value- if the opening was successful.</returns>
bool SqliteDatabase::open()
{
	vector<Question> vecQuest = fetchQuestions(AMOUNT_QUESTIONS);
	vector<string> answers = vector<string>();
	list<User>* users = nullptr;
	string baseCommand = "INSERT INTO QUESTIONS(QUESTION, CATEGORY, DIFFICULTY, CORRECT_ANSWER, ANSWER1, ANSWER2, ANSWER3) VALUES ";
	string command = "";
	//Opening/Creating the database file
	int res = sqlite3_open(DB_FILE_NAME.c_str(), &this->m_db);
	if (res != SQLITE_OK)
	{
		this->m_db = nullptr;
		return false;
	}

	//Creating/Opening the tables of the database
	try
	{
		//Going over the command to create the DB
		for (int i = 0; i < AMOUNT_TABLES; i++)
		{
			this->runSqlCommand(COMMANDS_CREATING_DB[i]);
		}
	}
	catch (const std::exception& excp)
	{
		std::cout << excp.what() << std::endl;
		return false;
	}

	//Inserting the questions into the database
	//Going over the vector of the questions
	for (auto i = vecQuest.begin(); i != vecQuest.end(); i++)
	{
		answers = i->getAnswers();
		command = baseCommand;
		command += "('" + i->getQuestion() + "', '";
		command += i->getCategory() + "', '" + i->getDifficulty() + "', '";
		command += i->getRightAnswer() + "', '" + answers[0] + "', '" + answers[1] + "', '" + answers[2];
		command += "');";

		this->runSqlCommand(command);
	}
	vecQuest.~vector();

	//Getting all the users to initialize the SCORES table
	command = "SELECT * FROM USERS;";
	users = runSqlCommand<User>(command);

	baseCommand = "INSERT INTO SCORES (USER_ID, SCORE) VALUES";
	//Going over all the users
	for (auto i = users->begin(); i != users->end(); i++)
	{
		command = baseCommand + "(" + to_string(i->getId()) + ", 0);";
		runSqlCommand(command);
	}
	users->~list();

	return true;
}

/// <summary>
/// The function closes the database file.
/// </summary>
/// <returns> a bool value- if the closing was successful or not.</returns>
bool SqliteDatabase::close()
{
	string command = "DELETE FROM QUESTIONS;";
	this->runSqlCommand(command);
	command = "DELETE FROM SCORES;";
	this->runSqlCommand(command);
	sqlite3_close(this->m_db);
	this->m_db = nullptr;
	return true;
}

/// <summary>
/// The function checks if a certain user is already 
/// exist in the database or not.
/// </summary>
/// <param name="username"> The username of the User to check</param>
/// <returns>an integer variable- if the user exist, not exist or if there was an error.</returns>
int SqliteDatabase::doesUserExist(const string username)
{
	list<User>* userList = nullptr;
	string command = "SELECT * FROM USERS WHERE USERNAME LIKE '" + username + "';";

	try
	{
		userList = this->runSqlCommand<User>(command);
	}
	catch (const std::exception& excp)
	{
		std::cout << excp.what() << std::endl;
		userList->~list();
		return ERROR_CODE;
	}

	//The user was found
	if (!userList->empty())
	{
		userList->~list();
		return USER_EXIST;
	}
	userList->~list();
	return USER_NOT_EXIST;

}

/// <summary>
/// The function checks if a certain user has a 
/// specific password.
/// </summary>
/// <param name="username"> The username of the User.</param>
/// <param name="password"> The password to check.</param>
/// <returns>an integer variable- if the password matches, does not matches or if there was an error.</returns>
int SqliteDatabase::doesPasswordMatch(const string username, const string password)
{
	list<User>* usersList = nullptr;
	string command = "SELECT * FROM USERS WHERE USERNAME LIKE '" + username + "' AND PASSWORD LIKE '" + password + "';";

	try
	{
		usersList = this->runSqlCommand<User>(command);
	}
	catch (const std::exception& excp)
	{
		std::cout << excp.what() << std::endl;
		delete usersList;
		return ERROR_CODE;
	}

	//There is a User with this username&password
	if (!usersList->empty())
	{
		usersList->~list();
		return PASSWORD_MATCH;
	}
	usersList->~list();
	return PASSWORD_NOT_MATCH;
}

/// <summary>
/// The function adds another user to the database.
/// </summary>
/// <param name="user">The user to insert</param>
/// <returns>an integer value- if the user already exsit, if there was
/// an error or if the adding was OK.</returns>
int SqliteDatabase::addNewUser(const User& user)
{
	string command = "INSERT INTO USERS(USERNAME, PASSWORD, EMAIL, ADDRESS, PHONE_NUM, BORN_DATE) VALUES ";
	command += "('" + user.getUsername() + "', '" + user.getPassword() + "', '" + user.getEmail() + "', '"
		+ user.getAddress() + "', '" + user.getPhoneNum() + "', '" + user.getBornDate() + "'); ";

	if (USER_EXIST == doesUserExist(user.getUsername()))
	{
		return USER_EXIST;
	}
	this->runSqlCommand(command);

	//Initializing the statistic row of the new user
	command = "INSERT INTO STATISTICS(USER_ID, HIGH_SCORE, AMOUNT_CORRECT_ANSWERS, AVERAGE_ANSWER_TIME, TOTAL_AMOUNT_ANSWERS, AMOUNT_GAMES) VALUES ";
	command += "((SELECT ID FROM USERS WHERE USERNAME LIKE '" + user.getUsername() + "') , 0, 0, 0, 0, 0);";
	this->runSqlCommand(command);

	command = "INSERT INTO SCORES(USER_ID, SCORE) VALUES ((SELECT ID FROM USERS WHERE USERNAME LIKE '" + user.getUsername() + "'), 0);";
	this->runSqlCommand(command);

	return OK_CODE;
}

/// <summary>
/// The function extracts from the database a 
/// certain amount of questions.
/// </summary>
/// <param name="amountQuestions"> The amount of questions to extracts.</param>
/// <returns> The list of the questions.</returns>
list<Question>& SqliteDatabase::getQuestions(const int amountQuestions)
{
	string command = "SELECT * FROM QUESTIONS LIMIT " + to_string(amountQuestions) + ";";
	return *this->runSqlCommand<Question>(command);
}

/// <summary>
/// The function runns an SQL command on the database.
/// </summary>
/// <param name="command"> The command to run.</param>
void SqliteDatabase::runSqlCommand(const string command)
{
	char* errMsg = nullptr;
	int result = sqlite3_exec(this->m_db, command.c_str(), nullptr, nullptr, &errMsg);

	if (result != SQLITE_OK)
	{
		throw std::exception(errMsg);
	}
}

int SqliteDatabase::callbackIntegers(void* data, int argc, char** argv, char** azColName)
{
	//Going over the array of the arguments
	for (int i = 0; i < argc; i++)
	{
		(static_cast<list<int>*>(data))->push_back(atoi(argv[0]));
	}
	return OK_CODE;
}

/// <summary>
/// The functions extracts the data from an array for a User list.
/// </summary>
/// <param name="data"> The new destination of the data</param>
/// <param name="argc"> The amount of the arguments</param>
/// <param name="argv"> The arguments to move</param>
/// <param name="azColName"> The names of the columns of the data</param>
/// <returns> a integer value- if the extraction was successful.</returns>
int SqliteDatabase::callbackUsers(void* data, int argc, char** argv, char** azColName)
{
	string username = "", password = "", email = "", address = "", phoneNum = "", bornDate = "";
	int id = 0;
	User currUser = User(id, username, password, email, address, phoneNum, bornDate);

	//Going over the arguments
	for (int i = 0; i < argc; i++)
	{
		if (ID_FIELD == string(azColName[i]))
		{
			id = atoi(argv[i]);
		}
		else if (USERNAME_FIELD == string(azColName[i]))
		{
			username = argv[i];

		}
		else if (PASSWORD_FIELD == string(azColName[i]))
		{
			password = argv[i];
		}
		else if (EMAIL_FIELD == string(azColName[i]))
		{
			email = argv[i];
		}
		else if (ADDRESS_FIELD == string(azColName[i]))
		{
			address = argv[i];
		}
		else if (PHONE_NUM_FIELD == string(azColName[i]))
		{
			phoneNum = argv[i];
		}
		else if (BORN_DATE_FIELD == string(azColName[i]))
		{
			bornDate = argv[i];

			//Adding another user to the list
			currUser = User(id, username, password, email, address, phoneNum, bornDate);
			(static_cast<list<User>*>(data))->push_back(currUser);
		}
	}
	return OK_CODE;
}

/// <summary>
/// The functions extracts the data from an array for a User list.
/// </summary>
/// <param name="data"> The new destination of the data</param>
/// <param name="argc"> The amount of the arguments</param>
/// <param name="argv"> The arguments to move</param>
/// <param name="azColName"> The names of the columns of the data</param>
/// <returns> a integer value- if the extraction was successful.</returns>
int SqliteDatabase::callbackQuestions(void* data, int argc, char** argv, char** azColName)
{
	string quesiton = "", rightAnswer = "", difficulty = "", category = "";
	vector<string> answers;
	Question currentQuestion;

	//Going over the arguments
	for (int i = 0; i < argc; i++)
	{
		if (QUESTION_FIELD == string(azColName[i]))
		{
			quesiton = argv[i];
		}
		else if (CATEGORY_FIELD == string(azColName[i]))
		{
			category = argv[i];
		}
		else if (DIFFICULTY_FIELD == string(azColName[i]))
		{
			difficulty = argv[i];
		}
		else if (CORRECT_ANSWER_FIELD == string(azColName[i]))
		{
			rightAnswer = argv[i];
		}
		else if (string(azColName[i]).find(ANSWER_FIELD))
		{
			answers.push_back(argv[i]);

			if (static_cast<unsigned long long>(AMOUNT_ANSWERS) - 1 == answers.size())//support compatibility with wider type
			{
				currentQuestion = Question(quesiton, answers, rightAnswer, category, difficulty);
				(static_cast<list<Question>*>(data))->push_back(currentQuestion);
			}
		}
	}
	return OK_CODE;
}

int SqliteDatabase::callbackFloat(void* data, int argc, char** argv, char** azColName)
{
	*(static_cast<float*>(data)) = static_cast<float>(std::stof(argv[0]));//support conversion from int to float (atoi returns int)
	return OK_CODE;
}

int SqliteDatabase::callbackInt(void* data, int argc, char** argv, char** azColName)
{
	*(static_cast<int*>(data)) = static_cast<int>(atoi(argv[0]));//support conversion from int to float (atoi returns int)
	return OK_CODE;
}

int SqliteDatabase::callbackString(void* data, int argc, char** argv, char** azColName)
{
	*(static_cast<string*>(data)) = argv[0];
	return OK_CODE;
}

/// <summary>
/// The function runns a command on the SQL database.
/// </summary>
/// <typeparam name="T"> The type of the arguments to return</typeparam>
/// <param name="command"> The command to run</param>
/// <returns> a list of T values- the results of the command.</returns>
template <class T>
list<T>* SqliteDatabase::runSqlCommand(const string command)
{
	list<T>* listOfData = new list<T>();
	char* errMsg = nullptr;
	int result = ERROR_CODE;

	if (typeid(T).name() == typeid(User).name()) // the command should return Users
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackUsers, listOfData, &errMsg);
	}
	if (typeid(T).name() == typeid(Question).name()) // the command should return Questions
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackQuestions, listOfData, &errMsg);
	}
	if (typeid(T).name() == typeid(int).name()) // the command should return integers
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackIntegers, listOfData, &errMsg);
	}

	if (result != SQLITE_OK)
	{
		delete listOfData;
		throw std::exception(errMsg);
	}

	return listOfData;
}

/// <summary>
/// The function runns a SQL command which certainly 
/// return only 1 argument from a simple type.
/// </summary>
/// <typeparam name="T"> The type of the argument to return.</typeparam>
/// <param name="command"> The command to run</param>
/// <returns> The result of the command on the database</returns>
template<class T>
T SqliteDatabase::runSqlCommandSingleOutput(const string command)
{
	T data;
	char* errMsg = nullptr;
	int result = 0;

	if (typeid(T).name() == typeid(string).name()) // the command should return a string
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackString, &data, &errMsg);
	}
	else if (typeid(T).name() == typeid(float).name()) // the command should return a float or int
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackFloat, &data, &errMsg);
	}
	else if (typeid(T).name() == typeid(int).name())
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackInt, &data, &errMsg);
	}

	if (result != SQLITE_OK)
	{
		throw std::exception(errMsg);
	}
	//if (data != nullptr)
	//{
	//	return data;
	//}
	return data;
}

/// <summary>
/// The function getts the average answer time of a
/// certain user.
/// </summary>
/// <param name="player"> The name of the user.</param>
/// <returns> The average time of the user.</returns>
float SqliteDatabase::getPlayerAverageAnswerTime(const string player)
{
	string command = "SELECT AVERAGE_ANSWER_TIME FROM STATISTICS WHERE USER_ID == ";
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + player + "');";

	return runSqlCommandSingleOutput<float>(command);
}

/// <summary>
/// The function getts the amount of the correct answers which the
/// user has.
/// </summary>
/// <param name="player"> The name of the user.s</param>
/// <returns> The amount of the correct answers.</returns>
int SqliteDatabase::getNumOfCorrectAnswers(const string player)
{
	string command = "SELECT AMOUNT_CORRECT_ANSWERS FROM STATISTICS WHERE USER_ID == ";
	int amount = 0;
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + player + "');";

	amount = runSqlCommandSingleOutput<int>(command);
	return amount;
}

/// <summary>
/// The function getts the amount of the answers which the
/// user has.
/// </summary>
/// <param name="player"> The name of the user.</param>
/// <returns> The amount of the answers.</returns>
int SqliteDatabase::getNumOfTotalAnswers(const string player)
{
	string command = "SELECT TOTAL_AMOUNT_ANSWERS FROM STATISTICS WHERE USER_ID == ";
	int amount = 0;
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + player + "');";

	amount = runSqlCommandSingleOutput<int>(command);
	return amount;
}

/// <summary>
/// The function getts the amount of the games which the
/// user has played.
/// </summary>
/// <param name="player"> The name of the user.</param>
/// <returns> The amount of the games.</returns>
int SqliteDatabase::getNumOfPlayerGames(const string player)
{
	string command = "SELECT AMOUNT_GAMES FROM STATISTICS WHERE USER_ID == ";
	int amount = 0;
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + player + "');";

	amount = runSqlCommandSingleOutput<int>(command);
	return amount;
}

/// <summary>
/// The function getts the last score of a single user.
/// </summary>
/// <param name="player"> The user's name</param>
/// <returns> His last score</returns>
int SqliteDatabase::getPlayerScore(const string player)
{
	//string command = "SELECT SCORE FROM SCORES WHERE USER_ID == ";
	string command = "SELECT HIGH_SCORE FROM STATISTICS WHERE USER_ID == ";
	int score = 0;
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + player + "');";
	if (!doesUserExist(player))
	{
		return USER_NOT_EXIST;
	}
	score = runSqlCommandSingleOutput<int>(command);
	return score;
}

/// <summary>
/// The function getts the highest scores in the database.
/// </summary>
/// <returns> The highest scores.</returns>
vector<string>& SqliteDatabase::getHighScores()
{
	string command = "SELECT USER_ID FROM STATISTICS ORDER BY HIGH_SCORE DESC LIMIT " + to_string(AMOUNT_HIGH_SCORES) + ";";
	list<int>* idBestScores = runSqlCommand<int>(command);
	int currentScore = 0;
	string currentName = "";
	vector<string>* theBestScores = new vector<string>();

	//Going over the list of the id of the best scores
	for (auto i = idBestScores->begin(); i != idBestScores->end(); i++)
	{
		//Getting the Name of the current best user with its id
		command = "SELECT USERNAME FROM USERS WHERE ID == " + to_string(*i) + ";";
		currentName = runSqlCommandSingleOutput<string>(command);

		//Getting the Name of the current best user with its id
		command = "SELECT HIGH_SCORE FROM STATISTICS WHERE USER_ID == " + to_string(*i) + "; ";
		currentScore = runSqlCommandSingleOutput<int>(command);

		//Pushing the results to the vector
		theBestScores->push_back("'" + currentName + "'");
		theBestScores->push_back(to_string(currentScore));
	}
	idBestScores->~list();
	return *theBestScores;
}
