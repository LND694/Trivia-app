#include "OTPCryptoAlgorithm.h"

string OTPCryptoAlgorithm::encrypt(const string message, const byte* key) const
{
    string encryptedText = "";

    //Doing xor between the message and the key
    for (int i = 0; i < message.length(); i++)
    {
        encryptedText += (unsigned char)message[i] ^ (unsigned char)key[i];
    }
    return encryptedText;
}

string OTPCryptoAlgorithm::decrypt(const string message, const byte* key) const
{
    string realText = "";
    string decryptedText = "";

    //Doing xor between the message and the key
    for (int i = 0; i < message.length(); i++)
    {
        decryptedText += (unsigned char)message[i] ^ (unsigned char)key[i];
    }

    for (const auto& i : decryptedText)//delete the hex chars after the message
    {
        if (static_cast<unsigned char>(i) <= 127)
        {
            realText += i;
        }
    }
    return realText;//the text without the hex chars
}


