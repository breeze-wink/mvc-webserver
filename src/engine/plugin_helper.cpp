#include "plugin_helper.h"
#include "Logger.h"
#include "system.h"
#include <dlfcn.h>

using namespace breeze::engine;
using namespace breeze::utility;

bool PluginHelper::load(const string& plugin)
{
    if (plugin.empty())
    {
        log_error("load plugin error: plugin is empty");
        return false;
    }
    if (m_plugins.find(plugin) != m_plugins.end())
    {
        return true;
    }
    auto sys = Singleton<System>::Instance();
    string filename = sys -> get_root_path() + "plugin/lib" + plugin; 

    void* handle = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_LAZY);
    if (handle == nullptr)
    {
        log_error("load plugin error: %s", dlerror());
        return false;
    }
    m_plugins[plugin] = handle;

    return true;
}

void PluginHelper::unload(const string& plugin)
{
    if (plugin.empty())
    {
        log_error("unload plugin error: plugin is empty");
        return;
    }

    auto it = m_plugins.find(plugin);
    if (it == m_plugins.end())
    {
        log_error("unload plugin error: plugin not found");
        return;
    }
    dlclose(it -> second);
    m_plugins.erase(it);
}

void* PluginHelper::symbol(const string& plugin, const string& symbol)
{
    auto it = m_plugins.find(plugin);
    if (it == m_plugins.end())
    {
        if (!load(plugin))
        {
            return nullptr;
        }
        it = m_plugins.find(plugin);
    }

    void * func = dlsym(it -> second, symbol.c_str());

    if (func == nullptr)
    {
        log_error("undefined symbol: plugin = %s symbol = %s", plugin.c_str(), symbol.c_str());
    }

    return func;
}