#include "AesPlugin.h"
#include "PluginsManager.h"
#include "AesDefine.h"

namespace Util
{
	AesPlugin* plugin;
	extern "C"
	{
		void AES_PUBLIC DllInstall()
		{
			printf("aes install\n");
			plugin = new AesPlugin();
			PluginsManager::GetInstance()->installPlugin(MODULE, plugin);
		}
		void AES_PUBLIC DllUninstall()
		{
			printf("aes uninstall\n");
			PluginsManager::GetInstance()->uninstallPlugin(MODULE);
			delete plugin;
		}
	}
}