#include "OTPCryptoAlgorithm.h"

string OTPCryptoAlgorithm::encrypt(const string message, const byte* key) const
{
    return this->decrypt(message, key);
}

string OTPCryptoAlgorithm::decrypt(const string message, const byte* key) const
{
    string realText;
    //CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key), OTP_KEY_SIZE);
    //CryptoPP::SecByteBlock encryptedBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());

    //CryptoPP::SecByteBlock decryptedBytes(encryptedBytes.size());
    //CryptoPP::xorbuf(decryptedBytes, encryptedBytes, keyBytes, encryptedBytes.size());

    //string decryptedText(decryptedBytes.begin(), decryptedBytes.end());
    string decryptedText = "";

    //Doing xor between the message and the key
    for (int i = 0; i < message.length(); i++)
    {
        decryptedText += (unsigned char)message[i] ^ (unsigned char)key[i];
    }

    for (const auto& i : decryptedText)//delete the hex chars after the message
    {
        realText += i;
        if (static_cast<unsigned char>(i) > 127)
        {
            break;
        }

    }
    return realText;//the text without the hex chars
}


