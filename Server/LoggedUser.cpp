#include "LoggedUser.h"

/// <summary>
/// C'tor of class LoggedUser.
/// </summary>
/// <param name="username"> The username of the logged user.</param>
LoggedUser::LoggedUser(const string username):
    m_username(username)
{
}

/// <summary>
/// The function getts the username of the user. 
/// </summary>
/// <returns> The username of the user</returns>
string LoggedUser::getUsername() const
{
    return this->m_username;
}

/// <summary>
/// The operator '<' checks if this logged user
/// is smaller than the other logged user.
/// </summary>
/// <param name="other"> The other logged user.</param>
/// <returns> If his logged user is smaller or not.</returns>
bool LoggedUser::operator<(const LoggedUser& other) const
{
    return this->m_username < other.m_username;
}
