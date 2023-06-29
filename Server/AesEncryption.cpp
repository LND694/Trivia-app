#include "AesEncryption.h"

/// <summary>
/// encrypt given message
/// </summary>
/// <param name="message"> the message to encrypt</param>
/// <param name="key"> the encryption key </param>
/// <param name="iv"> the intialization vector encryption</param>
/// <returns> the encrypted string</returns>
string AesEncryption::encrypt(const string message, const byte* key) const
{
    string ciphertext;

    // Convert std::byte* to CryptoPP::byte*
    const CryptoPP::byte* aesKey = reinterpret_cast<const CryptoPP::byte*>(key);
    const CryptoPP::byte* aesIV = reinterpret_cast<const CryptoPP::byte*>(_iv);

    CryptoPP::AES::Encryption aesEncryption(aesKey, AES_KEY_SIZE);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncryption, aesIV);

    CryptoPP::StreamTransformationFilter encryptor(cbcEncryption,
        new CryptoPP::StringSink(ciphertext),
        CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING
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
string AesEncryption::decrypt(const string message, const byte* key) const
{
    string decryptedText;
    // Convert key to SecByteBlock
    CryptoPP::SecByteBlock aesKey(reinterpret_cast<const CryptoPP::byte*>(key), AES_KEY_SIZE);

    // Convert IV to SecByteBlock
    CryptoPP::SecByteBlock aesIV(reinterpret_cast<const CryptoPP::byte*>(this->_iv), CryptoPP::AES::BLOCKSIZE);

    // Decryption
    CryptoPP::AES::Decryption aesDecryption(aesKey, aesKey.size());
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecryption, aesIV);

    CryptoPP::ArraySource decryptor(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size(), true,
        new CryptoPP::StreamTransformationFilter(cbcDecryption,
            new CryptoPP::StringSink(decryptedText),
            CryptoPP::BlockPaddingSchemeDef::PKCS_PADDING
        )
    );

    return decryptedText;
}

void AesEncryption::setIv(const byte* iv)
{
    this->_iv = const_cast<byte*>(iv);
}

AesEncryption::AesEncryption()
{
}
