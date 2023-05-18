#pragma once
#include <iostream>
#include "User.h"
#include "sqlite3.h"
#include "global.h"
#include "Singleton.h"

using std::string;
enum RETURNED_CODES {
	ERROR_CODE = -1, OK_CODE = 0,
	USER_EXIST = 1, USER_NOT_EXIST = 2,
	PASSWORD_MATCH = 3, PASSWORD_NOT_MATCH = 4
};

class IDatabase : public Singleton
{
public:

	//Open&Close functions 
	virtual bool open() = 0;
	virtual bool close() = 0;

	//User functions
	virtual int doesUserExist(const string username) = 0;
	virtual int doesPasswordMatch(const string username, const string password) = 0;
	virtual int addNewUser(const User& user) = 0;
};