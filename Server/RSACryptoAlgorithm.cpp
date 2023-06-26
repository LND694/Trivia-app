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
    CryptoPP::RSAES_PKCS1v15_Encryptor encryptor(clientKey);
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
    CryptoPP::RSA::PublicKey* publicKey = new CryptoPP::RSA::PublicKey();

    // Load the XML string into a pugixml document
    pugi::xml_document doc;
    if (!doc.load_string(pemPublicKey.c_str()))
    {
        throw std::exception("Failed to parse XML string.");
    }

    // Extract modulus and exponent values from the XML
    pugi::xml_node rsaKeyValueNode = doc.child("RSAKeyValue");
    pugi::xml_node modulusNode = rsaKeyValueNode.child("Modulus");
    pugi::xml_node exponentNode = rsaKeyValueNode.child("Exponent");

    if (!modulusNode || !exponentNode)
    {
        throw std::exception("Modulus or exponent node not found in XML.");
    }

    std::string modulusValue = modulusNode.child_value();
    std::string exponentValue = exponentNode.child_value();

    // Convert the byte arrays to CryptoPP::Integer
    CryptoPP::Integer modulus(reinterpret_cast<const unsigned char*>(modulusValue.c_str()), modulusValue.length());
    CryptoPP::Integer exponent(reinterpret_cast<const unsigned char*>(exponentValue.c_str()), exponentValue.length());

    publicKey->Initialize(exponent, modulus);

    return *publicKey;
}
