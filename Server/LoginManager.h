#pragma once
#include "SqliteDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include "Request.h"
#include "Singleton.h"
#include "global.h"

using std::vector;

class LoginManager : Singleton
{
public:
	static LoginManager* getInstance(IDatabase* db);
	LoginManager(LoginManager& other) = delete;
	void operator=(const LoginManager& other) = delete;

	//Functions
	SignupRequest& signUp(const User& user);
	LoginRequest& login(const string username, const string password);
	void logOut(const string username);

protected:
	//Singleton fields
	static LoginManager* m_instance;
	static Lock m_lock;

	//C'tor
	LoginManager(IDatabase* db);
	
private:
	//Fields
	IDatabase* m_dataBase;
	vector<LoggedUser> m_loggedUsers;
};