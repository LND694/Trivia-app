#include "LoggedUser.h"

LoggedUser::LoggedUser(const string username):
    m_username(username)
{
}

string LoggedUser::getUserName()
{
    return this->m_username;
}
