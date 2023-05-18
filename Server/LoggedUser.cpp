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
string LoggedUser::getUsername()
{
    return this->m_username;
}
