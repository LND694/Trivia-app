#pragma once
#include "IDatabase.h"
#include "LoggedUser.h"
#include <vector>
#include "Request.h"

using std::vector;
class LoginManager
{
public:
	SignupRequest& signUp(const string email, const string password, const string username);
	LoginRequest& login(const string username, const string password);
	void signUp(const string username);
	
private:
	IDatabase* m_dataBase;
	vector<LoggedUser> m_loggedUsers;
};