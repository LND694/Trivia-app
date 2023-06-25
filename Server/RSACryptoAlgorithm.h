#pragma once
#include <fstream>
#include <filesystem>
#include <Windows.h>
#include "CryptoAlgorithm.h"
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/base64.h>

using std::ofstream;
using std::filesystem::remove_all;

constexpr int MAX_SIZE_KEY = 32;
const string DIRECTORY_PATH = "C:/Keys";
const string SERVER_KEY_FILE = "ServerKey.txt";

class RSACryptoAlgorithm : public CryptoAlgorithm
{
public:
	RSACryptoAlgorithm();
	string encrypt(const string message, const string key) const override;
	string decrypt(const string message, const string key) const override;

private:
	CryptoPP::RSA::PublicKey m_publicKey;
	CryptoPP::RSA::PrivateKey m_privateKey;

	//Help function
	static void deletePreviousFile(const string filename);
};