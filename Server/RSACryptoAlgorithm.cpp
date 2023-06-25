#include "RSACryptoAlgorithm.h"

RSACryptoAlgorithm::RSACryptoAlgorithm():
	CryptoAlgorithm()
{
    BOOL result = false;
    std::string publicKeyStr = "";

    //Creating the random seed for the public and private keys
    CryptoPP::AutoSeededRandomPool rng;
	this->m_privateKey.GenerateRandomWithKeySize(rng, MAX_SIZE_KEY);
	this->m_publicKey = CryptoPP::RSA::PublicKey(this->m_privateKey);

    //Making a string from the public key
    CryptoPP::StringSink publicKeySink(publicKeyStr);
    this->m_publicKey.DEREncode(publicKeySink);


    result = CreateDirectoryA(DIRECTORY_PATH.c_str(), NULL);
    //Creating the directory of the keys
    if (result == ERROR && GetLastError() != ERROR_ALREADY_EXISTS)
    {
        throw std::exception("Can not create directory of keys!");
    }

    deletePreviousFile(DIRECTORY_PATH + "/" + SERVER_KEY_FILE);

    //Creating the file of the public key and inserting the public key to there
    std::ofstream file(DIRECTORY_PATH + "/" + SERVER_KEY_FILE, std::ios::out ); // Open file in output mode

    if (!file.is_open())
    {
        throw std::exception("Can not share the public key!");
    }
    file << publicKeyStr;
    file.close();

}

string RSACryptoAlgorithm::encrypt(const string message, const string key) const
{
	string encryptedText = "";
    CryptoPP::RSAES_OAEP_SHA_Encryptor encryptor(this->m_publicKey);
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::StringSource(message, true, 
        new CryptoPP::PK_EncryptorFilter(rng, encryptor, 
            new CryptoPP::StringSink(encryptedText)));

    return encryptedText;
}

string RSACryptoAlgorithm::decrypt(const string message, const string key) const
{
    string decryptedText = "";
    CryptoPP::RSAES_OAEP_SHA_Decryptor decryptor(this->m_privateKey);
    CryptoPP::AutoSeededRandomPool rng;

    CryptoPP::StringSource(message, true,
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