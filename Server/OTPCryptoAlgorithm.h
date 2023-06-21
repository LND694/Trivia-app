#pragma once
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include "CryptoAlgorithm.h"


class OTPCryptoAlgorithm : public CryptoAlgorithm
{
public:
	string encrypt(string message, string key) const override;
	string decrypt(string message, string key) const override;
};