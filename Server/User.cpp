#include "User.h"

/// <summary>
/// C'tor of class User.
/// </summary>
/// <param name="id"> The id of the user.</param>
/// <param name="username"> The username of the user.</param>
/// <param name="password"> The password of the user.</param>
/// <param name="email"> The email of the user.</param>
/// <param name="address"> The address of the user.</param>
/// <param name="phone"> The phone of the user.</param>
/// <param name="bornDate"> The born date of the user.</param>
User::User(const int id, const string username, const string password, const string email, const string address, const string phone, const string bornDate) :
m_id(id), m_username(username), m_password(password), m_email(email), m_addr(address), m_phone(phone), m_bornDate(bornDate)
{

}

/// <summary>
/// C'tor of class User.
/// </summary>
/// <param name="username"> The username of the user.</param>
/// <param name="password"> The password of the user.</param>
/// <param name="email"> The email of the user.</param>
/// <param name="address"> The address of the user.</param>
/// <param name="phoneNum"> The phone of the user.</param>
/// <param name="bornDate"> The born date of the user.</param>
User::User(const string username, const string password, const string email, const string address, const string phoneNum, const string bornDate):
	User(0, username, password, email, address, phoneNum, bornDate)
{

}

/// <summary>
/// C'tor of class User.
/// </summary>
/// <param name="signupReq"> The request with the data
/// of the new user.</param>
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

