#pragma once
#include "MongoDatabase.h"


class CryptoAlgorithm
{
protected:
	virtual string encrypt(string message) = 0;
	virtual string decrypt(string message) = 0;
	IDatabase* db;
};

