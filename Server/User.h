#pragma once
#include <iostream>
#include "Request.h"


#define ID_FIELD "ID"
#define USERNAME_FIELD "USERNAME"
#define PASSWORD_FIELD "PASSWORD"
#define EMAIL_FIELD "EMAIL"
#define ADDRESS_FIELD "ADDRESS"
#define PHONE_NUM_FIELD "PHONE_NUM"
#define BORN_DATE_FIELD "BORN_DATE"

constexpr int LEN_PASSWORD = 8;
constexpr int DEFAULT_ID = -1;

class User
{
private:
	//Fields
	int m_id = DEFAULT_ID;
	string m_username;
	string m_password;
	string m_email;
	string m_addr;
	string m_phone;
	string m_bornDate;

public:
	//C'tors
	User(const int id, const string username, const string password, const string email,
		const string address, const string phone, const string bornDate);
	User(const string username, const string password, const string email,
		const string address, const string phoneNum, const string bornDate);
	User(const SignupRequest& signupReq);


	//Getters
	int getId() const;
	string getUsername() const;
	string getPassword() const;
	string getEmail() const;
	string getAddress() const;
	string getPhoneNum() const;
	string getBornDate() const;
};