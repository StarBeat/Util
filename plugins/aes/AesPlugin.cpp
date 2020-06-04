#include "AesPlugin.h"
#include <stdio.h>
#include "EncryptionManager.h"
#include "AesEncryption.h"
namespace Util
{
AesPlugin::AesPlugin()
{
}

void AesPlugin::install()
{
	printf("AesPlugin::install\n");
	EncryptionManager::GetInstance()->aes = new AesEncryption();
}

void AesPlugin::initialize()
{
}

void AesPlugin::termination()
{
}

void AesPlugin::uninstall()
{
}
}
