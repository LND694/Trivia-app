#pragma once
#include "IDatabase.h"


class StatisticsManager
{
public:
	//C'tor&D'tor
	StatisticsManager(IDatabase* db);
	~StatisticsManager();

	//Fucntions
	vector<string>& getHighScore();
	vector<string>& getUserStatistics(const string username);
private:
	//Field
	IDatabase* m_database;

};

