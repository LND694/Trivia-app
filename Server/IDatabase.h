#pragma once
#include <iostream>
#include "sqlite3.h"


using std::string;

class IDatabase
{
public:
	//Open&Close functions 
	virtual bool open() = 0;
	virtual bool close() = 0;

	//User functions
	virtual int doesUserExist(const string username) = 0;
	virtual int doesPasswordMatch(const string username, const string password) = 0;
	virtual int addNewUser(const string username, const string password, const string email) = 0;
};