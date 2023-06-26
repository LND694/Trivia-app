#include "AesEncryption.h"

/// <summary>
/// encrypt given message
/// </summary>
/// <param name="message"> the message to encrypt</param>
/// <param name="key"> the encryption key </param>
/// <param name="iv"> the intialization vector encryption</param>
/// <returns> the encrypted string</returns>
string AesEncryption::encrypt(const string message, const string key) const
{
    string ciphertext;
    // Convert key to SecByteBlock
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());

    // Convert IV to SecByteBlock
    CryptoPP::SecByteBlock aesIV(reinterpret_cast<const CryptoPP::byte*>(this->_iv.data()), this->_iv.size());

    // Encryption
    CryptoPP::AES::Encryption aesEncryption(aesKey, aesKey.size());
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, aesIV);

    // Set up encryption filters
    CryptoPP::StringSource encryptor(message, true,
        new CryptoPP::StreamTransformationFilter(cbcEncryption,
            new CryptoPP::StringSink(ciphertext),
            CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING
        )
    );

    return ciphertext;
}

/// <summary>
/// decrypt given message
/// </summary>
/// <param name="message"> the message to decrypt </param>
/// <param name="key"> the decryption key </param>
/// <param name="iv"> the intialization vector decryption </param>
/// <returns>decrypted string</returns>
string AesEncryption::decrypt(const string message, const string key) const
{
    string decryptedText;
    // Convert key to SecByteBlock
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());

    // Convert IV to SecByteBlock
    CryptoPP::SecByteBlock aesIV(reinterpret_cast<const CryptoPP::byte*>(this->_iv.data()), this->_iv.size());

    // Decryption
    CryptoPP::AES::Decryption aesDecryption(aesKey, aesKey.size());
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, aesIV);

    // Set up decryption filters
    CryptoPP::StringSource decryptor(message, true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(decryptedText),
            CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING
        )
    );

    return decryptedText;
}

void AesEncryption::setIv(const string iv)
{
    this->_iv = iv;
}

AesEncryption::AesEncryption()
{
}
