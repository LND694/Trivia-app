#include "User.h"

User::User(const int id, const string username, const string password, const string email, const string address, const string phone, const string bornDate) :
m_id(id), m_username(username), m_password(password), m_email(email), m_addr(address), m_phone(phone), m_bornDate(bornDate)
{

}

User::User(const string username, const string password, const string email, const string address, const string phoneNum, const string bornDate):
	m_username(username), m_password(password), m_email(email), m_addr(address), m_phone(phoneNum), m_bornDate(bornDate)
{

}

User::User(const SignupRequest& signupReq) :
	m_username(signupReq.username), m_password(signupReq.password), m_email(signupReq.email),
	m_addr(signupReq.address), m_phone(signupReq.phoneNum), m_bornDate(signupReq.bornDate)
{

}

int User::getId() const
{
	return this->m_id;
}

string User::getUsername() const
{
	return this->m_username;
}

string User::getPassword() const
{
	return this->m_password;
}

string User::getEmail() const
{
	return this->m_email;
}

string User::getAddress() const
{
	return this->m_addr;
}

string User::getPhoneNum() const
{
	return this->m_phone;
}

string User::getBornDate() const
{
	return this->m_bornDate;
}

