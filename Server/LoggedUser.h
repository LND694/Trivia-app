#pragma once
#include <string>

using std::string;


class LoggedUser
{
public:
	//C'tor
	LoggedUser(const string username);

	//Getter
	string getUsername() const;

private:
	//Field
	string m_username;
};

