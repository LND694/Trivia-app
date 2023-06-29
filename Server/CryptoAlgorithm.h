#pragma once
#include "global.h"

using std::byte;

class CryptoAlgorithm
{
public:
	virtual string encrypt(const string message, const byte* key) const = 0;
	virtual string decrypt(const string message, const byte* key) const = 0;
	virtual void setIv(const byte* iv);
	Buffer& convertToBuffer(const string str);
	string convertToString(const Buffer& buf);
};

