#pragma once
#include "Singleton.h"
#include "plugin.h"
#include <map>

using namespace breeze::utility;

namespace breeze::engine
{
    //create 和 destroy的函数指针
    typedef Plugin* (*create_plugin)();
    typedef void (*destroy_plugin)(Plugin* plugin);

    class PluginHelper
    {
        SINGLETON(PluginHelper);
    public:

        bool load(const string& plugin);
        void unload(const string& plugin);
        void* symbol(const string& plugin, const string& symbol);

    private:
        std::map<string, void*> m_plugins;
    };
}