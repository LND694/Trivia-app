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

	bool operator<(const LoggedUser& other) const;

private:
	//Field
	string m_username;
};

