#pragma once
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include "CryptoAlgorithm.h"

constexpr int KEY_SIZE = 1024;

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
public:
	OTPCryptoAlgorithm();
	string encrypt(string message) override;
	string decrypt(string message, string key) override;
	string GenerateKey();

private:
	static vector<string> keys;
};