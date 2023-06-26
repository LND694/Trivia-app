#pragma once
#include "global.h"

class CryptoAlgorithm
{
public:
	virtual string encrypt(const string message, const string key) const = 0;
	virtual string decrypt(const string message, const string key) const = 0;
	virtual void setIv(const string iv);
	Buffer& convertToBuffer(const string str);
	string convertToString(const Buffer& buf);
};

