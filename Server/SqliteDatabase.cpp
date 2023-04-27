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
	const string baseCommand = "INSERT INTO QUESTIONS(QUESTION, CATEGORY, DIFFICULTY, CORRECT_ANSWER, ANSWER1, ANSWER2, ANSWER3) VALUES ";
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
		command +="('" + i->getQuestion() + "', '";
		command += i->getCategory() + "', '" + i->getDifficulty() + "', '";
		command += i->getRightAnswer() + "', '" + answers[0] + "', '" + answers[1] + "', '" + answers[2];
		command += "');";

		this->runSqlCommand(command);
	}
	vecQuest.clear();

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
		delete userList;
		return ERROR_CODE;
	}

	//The user was found
	if (!userList->empty())
	{
		delete userList;
		return USER_EXIST;
	}
	delete userList;
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
		delete usersList;
		return PASSWORD_MATCH;
	}
	delete usersList;
	return PASSWORD_NOT_MATCH;
}

/// <summary>
/// The function adds another user to the database.
/// </summary>
/// <param name="username"> The username of the new user</param>
/// <param name="password"> The password of the new user</param>
/// <param name="email"> The email of the new user</param>
/// <returns>an integer value- if the user already exsit, if there was
/// an error or if the adding was OK.</returns>
int SqliteDatabase::addNewUser(const User& user)
{
	string command = "INSERT INTO USERS(USERNAME, PASSWORD, EMAIL, ADDRESS, PHONE_NUM, BORN_DATE) VALUES ";
	command += "('" + user.getUsername() + "', '" + user.getPassword() + "', '"+ user.getEmail() + "', '" 
		+ user.getAddress() + "', '" + user.getPhoneNum() + "', '" + user.getBornDate() + "'); ";

	if (USER_EXIST == doesUserExist(user.getUsername()))
	{
		return USER_EXIST;
	}
	this->runSqlCommand(command);

	//Initializing the statistic row of the new user
	command = "INSERT INTO STATISTICS(USER_ID, AMOUNT_CORRECT_ANSWERS, AVERAGE_ANSWER_TIME, TOTAL_AMOUNT_ANSWERS, AMOUNT_GAMES INTEGER) VALUES ";
	command += "(SELECT ID FROM USERS WHERE USERNAME LIKE '" + user.getUsername() + "' , 0, 0, 0, 0);";
	this->runSqlCommand(command);

	return OK_CODE;
}

list<Question>& SqliteDatabase::getQuestions(const int amountQuestions)
{
	list<Question> lst;
	return lst;
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

			if (AMOUNT_ANSWERS - 1 == answers.size())
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
	*(static_cast<float*>(data)) = atoi(argv[0]);
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

template<class T>
T SqliteDatabase::runSqlCommandSingleOutput(const string command)
{
	T* data = nullptr;
	char* errMsg = nullptr;
	int result = 0;

	if (typeid(T).name() == typeid(string).name()) // the command should return a string
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackString, data, &errMsg);
	}
	else if (typeid(T).name() == typeid(float).name() || typeid(T).name() == typeid(int).name()) // the command should return a float or int
	{
		result = sqlite3_exec(this->m_db, command.c_str(), callbackFloat, data, &errMsg);
	}

	if (result != SQLITE_OK)
	{
		throw std::exception(errMsg);
	}
	return *data;
}

float SqliteDatabase::getPlayerAverageAnswerTime(const string player)
{
	string command = "SELECT AVERAGE_ANSWER_TIME FROM STATISTICS WHERE USER_ID == ";
	command += "(SELECT ID FROM USER WHERE USERNAME LIKE '" + player + "');";

	return runSqlCommandSingleOutput<float>(command);
}

int SqliteDatabase::getNumOfCorrectAnswers(const string player)
{
	string command = "SELECT AMOUNT_CORRECT_ANSWERS FROM STATISTICS WHERE USER_ID == ";
	command += "(SELECT ID FROM USER WHERE USERNAME LIKE '" + player + "');";

	return runSqlCommandSingleOutput<int>(command);
}

int SqliteDatabase::getNumOfTotalAnswers(const string player)
{
	string command = "SELECT TOTAL_AMOUNT_ANSWERS FROM STATISTICS WHERE USER_ID == ";
	command += "(SELECT ID FROM USER WHERE USERNAME LIKE '" + player + "');";

	return runSqlCommandSingleOutput<int>(command);
}

int SqliteDatabase::getNumOfPlayerGames(const string player)
{
	string command = "SELECT AMOUNT_GAMES FROM STATISTICS WHERE USER_ID == ";
	command += "(SELECT ID FROM USER WHERE USERNAME LIKE '" + player + "');";

	return runSqlCommandSingleOutput<int>(command);
}

int SqliteDatabase::getPlayerScore(const string player)
{
	return 0;
}

vector<string>& SqliteDatabase::getHighScores()
{
	string command = "SELECT HIGH_SCORE FROM STATISTICS ORDER BY HIGH_SCORE ASC LIMIT 3;";
	list<int>* bestScores = runSqlCommand<int>(command);
	vector<string>* theBestScores = new vector<string>();

	//Going over the list of the best scores
	for (auto i = bestScores->begin(); i != bestScores->end();i++)
	{
		theBestScores->push_back(to_string(*i));
	}
	return *theBestScores;
}
