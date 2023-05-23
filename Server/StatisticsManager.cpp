#include "StatisticsManager.h"


StatisticsManager::StatisticsManager(IDatabase* m_database)
    : m_database(m_database)
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
vector<string>& StatisticsManager::getHighScore()
{
    return this->m_database->getHighScores();
}

/// <summary>
/// get the stats of a user based on his name
/// </summary>
/// <param name="username"> the name of the user </param>
/// <returns> vector of strings</returns>
vector<string>& StatisticsManager::getUserStatistics(const string username)
{
    vector<string>* statistics = new vector<string>();
    statistics->push_back(std::to_string(this->m_database->getPlayerScore(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfCorrectAnswers(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfTotalAnswers(username)));
    statistics->push_back(std::to_string(this->m_database->getNumOfPlayerGames(username)));
    statistics->push_back(std::to_string(this->m_database->getPlayerAverageAnswerTime(username)));
    return *statistics;

}




