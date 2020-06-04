#include "EncryptionManager.h"
namespace Util
{
	template<> EncryptionManager* Singleton<EncryptionManager>::instance = new EncryptionManager();

}