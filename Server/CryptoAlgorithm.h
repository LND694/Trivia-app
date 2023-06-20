#pragma once
#include "global.h"

class CryptoAlgorithm
{
public:
	virtual string encrypt(string message, string key) const = 0;
	virtual string decrypt(string message, string key) const = 0;
	Buffer& convertToBuffer(string str);
	string convertToString(Buffer& buf);
};

