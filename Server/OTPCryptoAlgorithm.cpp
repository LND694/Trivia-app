#include "OTPCryptoAlgorithm.h"

string OTPCryptoAlgorithm::encrypt(string message)
{
    string key;
    // Convert plaintext and key to byte arrays
    CryptoPP::SecByteBlock plaintextBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());
    // Ensure key length matches plaintext length
    if (keyBytes.size() < plaintextBytes.size())
    {
        throw std::exception("Error: Key length must be at least as long as the plaintext!");
    }

    // Perform OTP encryption
    for (size_t i = 0; i < plaintextBytes.size(); i++)
    {
        plaintextBytes[i] ^= keyBytes[i];
    }

    // Convert ciphertext to hex string
    std::string ciphertext;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(ciphertext));
    encoder.Put(plaintextBytes.data(), plaintextBytes.size());
    encoder.MessageEnd();

    // Print the ciphertext
    std::cout << "Ciphertext: " << ciphertext << std::endl;

    return ciphertext;
}

string OTPCryptoAlgorithm::decrypt(string message, string key)
{

    // Convert ciphertext and key to byte arrays
    CryptoPP::SecByteBlock ciphertextBytes(reinterpret_cast<const CryptoPP::byte*>(message.data()), message.size());
    CryptoPP::SecByteBlock keyBytes(reinterpret_cast<const CryptoPP::byte*>(key.data()), key.size());

    // Perform OTP decryption
    for (size_t i = 0; i < ciphertextBytes.size(); i++)
    {
        ciphertextBytes[i] ^= keyBytes[i];
    }

    // Retrieve the decrypted plaintext
    string plaintext(reinterpret_cast<const char*>(ciphertextBytes.data()), ciphertextBytes.size());

    // Print the decrypted plaintext
    std::cout << "Decrypted plaintext: " << plaintext << std::endl;

    return plaintext;
}

string OTPCryptoAlgorithm::GenerateKey()
{
    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::SecByteBlock key(KEY_SIZE);
    rng.GenerateBlock(key, key.size());

    // encode to hex format
    std::string encodedKey;
    CryptoPP::HexEncoder encoder(new CryptoPP::StringSink(encodedKey));
    encoder.Put(key, key.size());
    encoder.MessageEnd();

    return encodedKey;
}
