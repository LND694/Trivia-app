#include "CryptoAlgorithm.h"
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/hex.h>

class AesEncryption : public CryptoAlgorithm
{
public:
	AesEncryption();
	string encrypt(const string message, const string key) const override;
	string decrypt(const string message, const string key) const override;
	void setIv(const string iv) override;
private:
	string _iv;
};
