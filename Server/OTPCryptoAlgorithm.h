#pragma once
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/osrng.h>
#include <cryptopp/filters.h>
#include "CryptoAlgorithm.h"

constexpr int KEY_SIZE = 1024;

class OTPCryptoAlgorithm : public CryptoAlgorithm
{
public:
	std::string encrypt(std::string message, std::string key) const override;
	std::string decrypt(std::string message, std::string key) const override;
	OTPCryptoAlgorithm();
};