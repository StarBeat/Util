#pragma once
#include "Encryption.h"
#include <iostream>
namespace Util
{
class AsymmetricEncryption:public Encryption
{
protected:
	uint8_t iv[16]= {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
public:
	virtual void setKey(uint8_t* key) = 0;
	virtual void decrypt(uint8_t* buf, int len) = 0;
	virtual void encrypt(uint8_t* buf, int len) = 0;
};
}