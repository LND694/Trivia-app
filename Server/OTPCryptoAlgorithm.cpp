#include "OTPCryptoAlgorithm.h"

string OTPCryptoAlgorithm::encrypt(std::string message, std::string key) const
{
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    CryptoPP::SecByteBlock plaintextBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());

    CryptoPP::SecByteBlock encryptedBytes(plaintextBytes.size());
    CryptoPP::xorbuf(encryptedBytes, plaintextBytes, keyBytes, plaintextBytes.size());

    string encryptedText(encryptedBytes.begin(),encryptedBytes.end());


    return encryptedText;
}

string OTPCryptoAlgorithm::decrypt(std::string message, std::string key) const
{
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    CryptoPP::SecByteBlock encryptedBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());

    CryptoPP::SecByteBlock decryptedBytes(encryptedBytes.size());
    CryptoPP::xorbuf(decryptedBytes, encryptedBytes, keyBytes, encryptedBytes.size());

    string decryptedText(decryptedBytes.begin(), decryptedBytes.end());

    return decryptedText;
}

OTPCryptoAlgorithm::OTPCryptoAlgorithm()
{
}

