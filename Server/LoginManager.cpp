#include "LoginManager.h"

/// <summary>
/// C'tor of class LoginManager
/// </summary>
/// <param name="db"> The database of the manger login</param>
LoginManager::LoginManager(IDatabase* db) :
    m_dataBase(db), m_loggedUsers(vector<LoggedUser>())
{

}

/// <summary>
/// signUp in the database
/// </summary>
/// <param name="email"> the email of the request</param>
/// <param name="password"> the password in the request</param>
/// <param name="username"> the username in the request</param>
/// <returns>SignUp request after register in the database</returns>
SignupRequest& LoginManager::signUp(const string email, const string password, const string username)
{
    int result = this->m_dataBase->doesUserExist(username);
    SignupRequest* signUpReq = nullptr;

    //Checking the result of the scan of the DB
    if (ERROR_CODE == result)
    {
        throw std::exception("Error using database!");
    }
    else if (USER_EXIST == result)
    {
        throw std::exception("The user is already exist.");
    }
    this->m_dataBase->addNewUser(username, password, email);
    //Creating a SignUpRequest
    signUpReq = new SignupRequest();
    signUpReq->email = email;
    signUpReq->password = password;
    signUpReq->username = username;


    return *signUpReq;
}


/// <summary>
/// login in the databse
/// </summary>
/// <param name="password"> the password in the request</param>
/// <param name="username"> the username in the request</param>
/// <returns>login request after register in the database</returns>
LoginRequest& LoginManager::login(const string username, const string password)
{
    int result = this->m_dataBase->doesUserExist(username);
    LoginRequest* logReq = nullptr;

    //Checking the result of the check of the users data
    if (ERROR_CODE == result)
    {
        throw std::exception("Error using database!");
    }
    else if (USER_NOT_EXIST == result)
    {
        throw std::exception("The user not exists!");
    }

    result = this->m_dataBase->doesPasswordMatch(username, password);

    //Checking the result of the check of the users data
    if (ERROR_CODE == result)
    {
        throw std::exception("Error using database!");
    }
    else if (PASSWORD_NOT_MATCH == result)
    {
        throw std::exception("Wrong password, can not login!");
    }

    this->m_loggedUsers.push_back(LoggedUser(username));

    //Creating a LoginRequest
    logReq = new LoginRequest();
    logReq->username = username;
    logReq->password = password;


    return* logReq;
}

/// <summary>
/// The function removes a user from the logged users.
/// </summary>
/// <param name="username">The username of the user to disconnect.</param>
void LoginManager::logOut(const string username)
{
    bool userWasRemoved = false;

    //Going over the logged users
    for (auto i = this->m_loggedUsers.begin(); i != this->m_loggedUsers.end() && !userWasRemoved; i++)
    {
        if (i->getUsername() == username) // the user to disconnect was found
        {
            this->m_loggedUsers.erase(i);
            userWasRemoved = true;
        }
    }
}
