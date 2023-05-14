#include "User.h"

/// <summary>
/// C'tor of class User
/// </summary>
/// <param name="id"> The id of the user.</param>
/// <param name="username"> The username of the user.</param>
/// <param name="password"> The password of the user.</param>
/// <param name="email"> The email of the user.</param>
User::User(const int id, const string username, const string password, const string email):
	m_id(id), m_username(username), m_password(password), m_email(email)
{
}
