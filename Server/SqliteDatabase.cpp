#include "SqliteDatabase.h"
#include "sqlite3.h"

SqliteDatabase::SqliteDatabase()
{
    this->open();
}

SqliteDatabase::~SqliteDatabase()
{
    this->close();
}

bool SqliteDatabase::open()
{
	//Opennig/Creating the database file
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

bool SqliteDatabase::close() 
{
	sqlite3_close(this->m_db);
	this->m_db = nullptr;
	return true;
}

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
	if (!userList->empty())
	{
		delete userList;
		return USER_EXIST;
	}
	delete userList;
	return USER_NOT_EXIST;

}

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

	if (!usersList->empty())
	{
		delete usersList;
		return PASSWORD_MATCH;
	}
	delete usersList;
	return PASSWORD_NOT_MATCH;
}

int SqliteDatabase::addNewUser(const string username, const string password, const string email)
{
	string command = "INSERT INTO USERS(USERNAME, PASSWORD, EMAIL) VALUES ";
	command += "('" + username + "', '" + password + "', '" + email + "');";

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

void SqliteDatabase::runSqlCommand(const string command)
{
	char* errMsg = nullptr;
	int result = sqlite3_exec(this->m_db, command.c_str(), nullptr, nullptr, &errMsg);

	if (result != SQLITE_OK)
	{
		throw std::exception(errMsg);
	}
}

int SqliteDatabase::callbackUsers(void* data, int argc, char** argv, char** azColName)
{
	string username = "", password = "", email = "";
	int id = 0;
	User currUser = User(id, username, password, email);
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

			currUser = User(id, username, password, email);
			((list<User>*)data)->push_back(currUser);
		}
	}
	return OK_CODE;
}

template <class T>
list<T>* SqliteDatabase::runSqlCommand(const string command)
{
	list<T>* listOfData = new list<T>();
	char* errMsg = nullptr;
	int result = ERROR_CODE;

	if (typeid(T).name() == typeid(User).name()) // the command should return Albums
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
