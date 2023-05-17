#pragma once
#include "IDatabase.h"
#include "User.h"
#include "sqlite3.h"
#include <list>


using std::list;



const string DB_FILE_NAME = "triviaDB.sqlite";
const string COMMANDS_CREATING_DB[] = {"CREATE TABLE IF NOT EXISTS USERS (ID INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, USERNAME TEXT NOT NULL, PASSWORD TEXT NOT NULL, EMAIL TEXDT NOT NULL);"};

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
	int addNewUser(const string username, const string password, const string email) override;

private:
	//Field
	sqlite3* m_db;

	//Help functions
	void runSqlCommand(const string command);
	static int callbackUsers(void* data, int argc, char** argv, char** azColName);
	template <class T>
	list<T>* runSqlCommand(const string command);
};
