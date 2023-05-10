#pragma once
#include <iostream>

using std::string;

#define USERNAME_FIELD "USERNAME"
#define ID_FIELD "ID"
#define PASSWORD_FIELD "PASSWORD"
#define EMAIL_FIELD "EMAIL"

class User
{
private:
	//Fields
	int m_id;
	string m_username;
	string m_password;
	string m_email;

public:
	//C'tor
	User(const int id, const string username, const string password, const string email);

};