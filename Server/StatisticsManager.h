#pragma once
#include "IDatabase.h"


class StatisticsManager
{
public:
	//C'tor&D'tor
	StatisticsManager(IDatabase* db);
	~StatisticsManager();

	//Functions
	vector<string>& getHighScore() const;
	vector<string>& getUserStatistics(const string username) const;
private:
	//Field
	IDatabase* m_database;

};

