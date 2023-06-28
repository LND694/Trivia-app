#pragma once
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include "CryptoAlgorithm.h"


constexpr int OTP_KEY_SIZE = 200;

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
public:
	string encrypt(const string message, const byte* key) const override;
	string decrypt(const string message, const byte* key) const override;
};