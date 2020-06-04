#pragma once
#include "Singleton.h"
#include "AsymmetricEncryption.h"
#include "Defines.h"

namespace Util 
{
class DLL_PUBLIC EncryptionManager : public Singleton<EncryptionManager>
{
public:
	AsymmetricEncryption* aes;

};
}
