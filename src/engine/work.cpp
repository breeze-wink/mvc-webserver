#include "work.h"
#include "plugin_helper.h"

using namespace breeze::engine;

Work::Work() : m_id(0), m_switch(false){}

Work::~Work()
{
    for (auto plugin : m_plugins)
    {
        const string& plugin_name = plugin -> get_name();
        destroy_plugin func = (destroy_plugin)Singleton<PluginHelper>::Instance() -> symbol(plugin_name, "destroy");
        func(plugin);
    }

}

void Work::append(Plugin* plugin)
{
    m_plugins.push_back(plugin);
}

bool Work::run(Context& ctx)
{
    for (auto it = m_plugins.begin(); it != m_plugins.end(); ++ it)
    {
        if ((*it) -> get_switch())
        {
            //将ctx再传递给各个插件
            if (!(*it) -> run(ctx))
            {
                break;
            }
        }
    }
    return true;
}

void Work::set_id(int id)
{
    m_id = id;
}
int Work::get_id()
{
    return m_id;
}

void Work::set_switch(bool flag)
{
    m_switch = flag;
}
bool Work::get_switch() const
{
    return m_switch;
}