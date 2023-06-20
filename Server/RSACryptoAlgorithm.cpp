#include "RSACryptoAlgorithm.h"

RSACryptoAlgorithm::RSACryptoAlgorithm():
	CryptoAlgorithm()
{
    // Generate RSA key pair
    const string FILE_KEY_PATH = DIRECTORY_PATH + "/" + SERVER_KEY_FILE;
    CryptoPP::AutoSeededRandomPool rng;
    this->m_privateKey.GenerateRandomWithKeySize(rng, MAX_SIZE_KEY);
    deletePreviousFile(FILE_KEY_PATH);


    // Export public key in PEM format
    this->m_publicKey = CryptoPP::RSA::PublicKey(this->m_privateKey);

    // Write the PEM encoded key to a file
    std::ofstream file(FILE_KEY_PATH);
    if (file.is_open())
    {
        file << "-----BEGIN PUBLIC KEY-----\n";
        file << publicKeyToString() << "\n";
        file << "-----END PUBLIC KEY-----\n";
        file.close();
    }
}

string RSACryptoAlgorithm::encrypt(const string message, const string key) const
{
	string encryptedText = "";
    CryptoPP::RSA::PublicKey& clientKey = loadPublicKeyFromPEM(key);
    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(clientKey);
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::StringSource strSrc(message, true, 
        new CryptoPP::PK_EncryptorFilter(rng, encryptor, 
            new CryptoPP::StringSink(encryptedText)));

    delete& clientKey;
    return encryptedText;
}

string RSACryptoAlgorithm::decrypt(const string message, const string key) const
{
    string decryptedText = "";
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(this->m_privateKey);
    CryptoPP::AutoSeededRandomPool rng;
    

    CryptoPP::StringSource strSrc(message, true,
        new CryptoPP::PK_DecryptorFilter(rng, decryptor,
            new CryptoPP::StringSink(decryptedText)));

    return decryptedText;
}

void RSACryptoAlgorithm::deletePreviousFile(const string filename)
{
    std::ifstream fileCheck(filename);
    if (fileCheck.good())  //the previous file exists
    {
        // File exists, so delete it
        fileCheck.close();
        std::remove(filename.c_str());
    }

}

string RSACryptoAlgorithm::publicKeyToString()
{
    // Encode the public key
    CryptoPP::ByteQueue queue;
    this->m_publicKey.DEREncode(queue);

    // Convert the byte queue to a string
    std::string encodedKey;
    CryptoPP::Base64Encoder encoder;
    queue.CopyTo(encoder);
    encoder.MessageEnd();
    CryptoPP::word64 size = encoder.MaxRetrievable();
    if (size)
    {
        encodedKey.resize(size);
        encoder.Get(reinterpret_cast<CryptoPP::byte*>(&encodedKey[0]), encodedKey.size());
    }

    return encodedKey;
}

CryptoPP::RSA::PublicKey& RSACryptoAlgorithm::loadPublicKeyFromPEM(const string pemPublicKey)
{
    std::istringstream iss(pemPublicKey);
    std::string base64EncodedKey;

    // Skip the PEM header
    std::getline(iss, base64EncodedKey);

    // Read the base64 encoded key
    std::getline(iss, base64EncodedKey);

    // Skip the PEM footer
    std::string line;
    while (std::getline(iss, line) && line != "-----END PUBLIC KEY-----");

    // Decode the base64 encoded key
    std::string derPublicKey;
    CryptoPP::StringSource ss(base64EncodedKey, true,
        new CryptoPP::Base64Decoder(new CryptoPP::StringSink(derPublicKey)));

    //Making the public key
    CryptoPP::RSA::PublicKey* publicKey = new CryptoPP::RSA::PublicKey();
    publicKey->Load(CryptoPP::FileSource(derPublicKey.c_str(), true).Ref());

    return *publicKey;
}
