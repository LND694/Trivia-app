#pragma once
#include "SqliteDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include "Request.h"

using std::vector;

class LoginManager
{
public:
	//C'tor
	LoginManager(IDatabase* db);

	//Functions
	SignupRequest& signUp(const string email, const string password, const string username);
	LoginRequest& login(const string username, const string password);
	void logOut(const string username);
	
private:
	//Fields
	IDatabase* m_dataBase;
	vector<LoggedUser> m_loggedUsers;
};