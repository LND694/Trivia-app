#include "SqliteDatabase.h"
#include "sqlite3.h"

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
/// The function opens the database file. 
/// If the file is not exsit it will create one with the tables.
/// </summary>
/// <returns>a bool value- if the opening was successful.</returns>
bool SqliteDatabase::open()
{
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
		this->runSqlCommand(COMMANDS_CREATING_DB[0]);
	}
	catch (const std::exception& excp)
	{
		std::cout << excp.what() << std::endl;
		return false;
	}
	return true;
}

/// <summary>
/// The function closes the database file.
/// </summary>
/// <returns> a bool value- if the closing was successful or not.</returns>
bool SqliteDatabase::close() 
{
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

	try
	{
		this->runSqlCommand(command);
	}
	catch (const std::exception& excp)
	{
		std::cout << excp.what() << std::endl;
		return ERROR_CODE;
	}
	return OK_CODE;
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

/// <summary>
/// The functions extracts the data from an array to another
/// variable.
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

	if (result != SQLITE_OK)
	{
		delete listOfData;
		throw std::exception(errMsg);
	}

	return listOfData;
}
