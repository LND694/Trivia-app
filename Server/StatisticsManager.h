#pragma once
#include "IDatabase.h"


class StatisticsManager
{
public:

	StatisticsManager(IDatabase* m_database);
	~StatisticsManager();
	vector<string>& getHighScore();
	vector<string>& getUserStatistics(const string username);
private:
	IDatabase* m_database;

};

