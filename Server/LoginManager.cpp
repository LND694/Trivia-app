#include "LoginManager.h"

LoginManager* LoginManager::m_instance = nullptr;
Lock LoginManager::m_lock;

/// <summary>
/// C'tor of class LoginManager
/// </summary>
/// <param name="db"> The database of the manger login</param>
LoginManager::LoginManager(IDatabase* db) :
    m_dataBase(db), m_loggedUsers(vector<LoggedUser>())
{

}

/// <summary>
/// The function checks if a user is already logged in.
/// </summary>
/// <param name="username"> The username to check</param>
/// <returns> if this user is logged or not.</returns>
bool LoginManager::isUserAlreadyLoggedIn(string username)
{
    //Going over the Logged Users
    for (const auto& i : this->m_loggedUsers)
    {
        if (i.getUsername() == username)
        {
            return true;
        }
    }
    return false;
}

/// <summary>
/// The function getts the instance of the class LoginManager.
/// </summary>
/// <param name="db">The database for the object LoginManager.</param>
/// <returns> the address of the instance</returns>
LoginManager* LoginManager::getInstance(IDatabase* db)
{
    lock_guard<Lock> lockGuard(m_lock);
    if (m_instance == nullptr)
    {
        m_instance = new LoginManager(db);
    }
    return m_instance;
}

/// <summary>
/// signUp in the database
/// </summary>
/// <param name="email"> the email of the request</param>
/// <param name="password"> the password in the request</param>
/// <param name="username"> the username in the request</param>
/// <returns>SignUp request after register in the database</returns>
SignupRequest& LoginManager::signUp(const User& user)
{
    int result = this->m_dataBase->doesUserExist(user.getUsername());
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
    this->m_dataBase->addNewUser(user);

    //Creating a SignUpRequest
    signUpReq = new SignupRequest();
    signUpReq->email = user.getEmail();
    signUpReq->password = user.getPassword();
    signUpReq->username = user.getUsername();
    signUpReq->address = user.getAddress();
    signUpReq->phoneNum = user.getPhoneNum();
    signUpReq->bornDate = user.getBornDate();


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
    else if (isUserAlreadyLoggedIn(username))
    {
        throw std::exception("This user is already logged in!");
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
    std::erase_if(this->m_loggedUsers, [username](LoggedUser& currentUsername){return username == currentUsername.getUsername(); });
}
