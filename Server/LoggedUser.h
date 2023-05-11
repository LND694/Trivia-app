#pragma once
#include <string>

using std::string;


class LoggedUser
{
public:
	string getUserName();

private:
	string m_username;
};

