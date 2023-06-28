#include "OTPCryptoAlgorithm.h"

string OTPCryptoAlgorithm::encrypt(const string message, const string key) const
{
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    CryptoPP::SecByteBlock plaintextBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());

    CryptoPP::SecByteBlock encryptedBytes(plaintextBytes.size());
    CryptoPP::xorbuf(encryptedBytes, plaintextBytes, keyBytes, plaintextBytes.size());

    string encryptedText(encryptedBytes.begin(),encryptedBytes.end());


    return encryptedText;
}

string OTPCryptoAlgorithm::decrypt(const string message, const string key) const
{
    string realText;
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    CryptoPP::SecByteBlock encryptedBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());

    CryptoPP::SecByteBlock decryptedBytes(encryptedBytes.size());
    CryptoPP::xorbuf(decryptedBytes, encryptedBytes, keyBytes, encryptedBytes.size());

    string decryptedText(decryptedBytes.begin(), decryptedBytes.end());

    for (const auto& i : decryptedText)//delete the hex chars after the message
    {
        realText += i;
        if (i == '\0')
        {
            break;
        }

    }
    return realText;//the text without the hex chars
}


