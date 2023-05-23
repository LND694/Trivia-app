#pragma once
#include "IDatabase.h"


class StatisticsManager
{
public:

	StatisticsManager(IDatabase* db);
	~StatisticsManager();
	vector<string>& getHighScore();
	vector<string>& getUserStatistics(const string username);
private:
	IDatabase* m_database;

};

