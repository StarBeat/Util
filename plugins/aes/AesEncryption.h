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
	void SetKey(uint8_t* key)override;
	void Decrypt(uint8_t* buf, int len)override;
	void Encrypt(uint8_t* buf, int len)override;
};
}