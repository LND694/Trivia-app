#include "CryptoAlgorithm.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

constexpr int AES_KEY_SIZE = 32;

class AesEncryption : public CryptoAlgorithm
{
public:
	AesEncryption();
	string encrypt(const string message, const byte* key) const override;
	string decrypt(const string message, const byte* key) const override;
	void setIv(const byte* iv) override;
private:
	byte* _iv;
};
