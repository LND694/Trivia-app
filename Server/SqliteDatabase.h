#pragma once
#include "IDatabase.h"
#include "User.h"
#include "sqlite3.h"
#include <list>


using std::list;


enum RETURNED_CODES{ERROR_CODE = -1, OK_CODE = 0,
	USER_EXIST = 1, USER_NOT_EXIST = 2, 
	PASSWORD_MATCH = 3, PASSWORD_NOT_MATCH = 4};
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
	//C'tor&D'tor
	SqliteDatabase();
	~SqliteDatabase();

	//Open&Close functions 
	bool open() override;
	bool close() override;

	//User functions
	int doesUserExist(const string username) override;
	int doesPasswordMatch(const string username, const string password) override;
	int addNewUser(const User& user) override;

private:
	//Field
	sqlite3* m_db;

	//Help functions
	void runSqlCommand(const string command);
	static int callbackUsers(void* data, int argc, char** argv, char** azColName);
	template <class T>
	list<T>* runSqlCommand(const string command);
};
