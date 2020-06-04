#pragma once
#include "Singleton.h"
#include "LibEntity.h"
#include "IPlugin.h"
#include <unordered_map>
namespace Util
{
class DLL_PUBLIC  PluginsManager : public Singleton<PluginsManager>
{
public:
	PluginsManager();
	virtual ~PluginsManager();
	void unload(LibEntity* lib);
	LibEntity* load(const std::string flname);
	void installPlugin(const std::string name, IPlugin* plugin);
	void uninstallPlugin(const std::string name);
	IPlugin* getPlugin(const std::string name);
private:
	std::unordered_map<std::string, IPlugin*> pluginsMap;
	std::unordered_map<std::string, LibEntity*> libsMap;
};
}
