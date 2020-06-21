#pragma once
#include "AsymmetricEncryption.h"
#include "aes.h"
namespace Util 
{
class AesEncryption : public AsymmetricEncryption
{
private:
	AES_ctx ctx;
	void setIv();
public:
	void setKey(uint8_t* key)override;
	void decrypt(uint8_t* buf, int len)override;
	void encrypt(uint8_t* buf, int len)override;
};
}