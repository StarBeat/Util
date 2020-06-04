#include "PluginsManager.h"
#include "Singleton.h"
#include "LibEntity.h"
#include "IPlugin.h"
#include <unordered_map>
namespace Util
{
using dllInstall = void(*)();
using dllUninstall = void(*)();
PluginsManager* Singleton<PluginsManager>::instance = new PluginsManager();

PluginsManager::PluginsManager()
{
}

PluginsManager::~PluginsManager()
{
	for (auto it : libsMap)
	{
		it.second->unload();
		delete it.second;
	}
}

void PluginsManager::unload(LibEntity* lib)
{
	dllUninstall fun = (dllUninstall)lib->getSymbol("DllUninstall");
	fun();
	lib->unload();
}

void PluginsManager::installPlugin(const std::string name, IPlugin* plugin)
{
	auto it = pluginsMap.find(name);
	if (it != pluginsMap.end())
	{
		//todo loge
	}
	else
	{
		pluginsMap[name] = plugin;
	}
}

void PluginsManager::uninstallPlugin(const std::string name)
{
	auto it = pluginsMap.find(name);
	if (it != pluginsMap.end())
	{
		pluginsMap.erase(it);
	}
}

IPlugin* PluginsManager::getPlugin(const std::string name)
{
	auto it = pluginsMap.find(name);
	if (it != pluginsMap.end())
	{
		return it->second;
	}
}

LibEntity* PluginsManager::load(const std::string name)
{
	auto it = libsMap.find(name);
	if (it != libsMap.end())
	{
		return it->second;
	}
	LibEntity* lib = new LibEntity(name);
	lib->load();
	libsMap[name] = lib;
	dllInstall fun =(dllInstall)lib->getSymbol("DllInstall");
	fun();
	return lib;
}
}