#pragma once
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include "CryptoAlgorithm.h"


class OTPCryptoAlgorithm : public CryptoAlgorithm
{
public:
	string encrypt(const string message, const string key) const override;
	string decrypt(const string message, const string key) const override;
};