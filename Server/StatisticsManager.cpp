#include "StatisticsManager.h"

/// <summary>
/// c`tor for the StatisticsManager
/// </summary>
/// <param name="db"> the database</param>
StatisticsManager::StatisticsManager(IDatabase* db)
    : m_database(db)
{
}

/// <summary>
/// d`tor for StatisticsManager
/// </summary>
StatisticsManager::~StatisticsManager()
{
    this->m_database = nullptr;
}

/// <summary>
/// get the best scores from the databse
/// </summary>
/// <returns> vector of strings </returns>
vector<string>& StatisticsManager::getHighScore() const
{
    return this->m_database->getHighScores();
}

/// <summary>
/// get the stats of a user based on his name
/// </summary>
/// <param name="username"> the name of the user </param>
/// <returns> vector of strings</returns>
vector<string>& StatisticsManager::getUserStatistics(const string username) const
{
    vector<string>* statistics = new vector<string>();
    statistics->push_back("'statistics':["+std::to_string(this->m_database->getPlayerScore(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
    statistics->push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username))+"]");
    return *statistics;

}

/// <summary>
/// The function adds a question from the user to the DB.
/// </summary>
/// <param name="question"> The question to insert.</param>
void StatisticsManager::addQuestion(const Question& question)
{
    this->m_database->insertQuestionToDB(question);
}




