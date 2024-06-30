#include "workflow.h"
#include "Logger.h"
#include "Singleton.h"
#include "plugin_helper.h"
#include "context.h"
#include "work.h"
// #include <memory>

using namespace breeze::engine;
using namespace breeze::xml;

bool Workflow::load(const string& config)
{

    Xml root;
    root.load(config);

    for (auto workinfo : root)
    {
        int id = workinfo.attr("id");
        const string &flag = workinfo.attr("switch");
        auto work = new Work();

        work -> set_id(id);
        work -> set_switch((flag == "on" ? true : false));

        if (!load_plugin(work, workinfo))
        {
            log_error("workflow load plugin error : work id = %d", id);
            return false;
        }

        m_works[id] = work;
    }
    log_info("load workflow success: %s", config.c_str());

    return true;
}

bool Workflow::run(int id, const string& input, string& output)
{
    auto it = m_works.find(id);
    if (it == m_works.end())
    {
        log_error("work not found: work id = %d", id);
        return false;
    }

    if (! it -> second -> get_switch())
    {
        log_error("work switch off: work id = %d", id);
        return false;
    }

    Context ctx;
    ctx.set("input", input);

    //以引用的形式将业务的信息用上下文传递给插件，插件进行相应操作，最后反馈一个output
    bool ret = it -> second -> run(ctx);

    if (! ret)
    {
        log_error("work run error, work_id = %d", id);
        return false;
    }

    output = ctx.get<string>("output");
    return true;
}

bool Workflow::load_plugin(Work* work, Xml& elem)
{
    for (auto pluginInfo : elem)
    {
        if (pluginInfo.name() != "plugin")
        {
            log_error("plugin not supported: %s", pluginInfo.name().c_str());
            return false;
        }
        const string& name = pluginInfo.attr("name");
        create_plugin func = (create_plugin)Singleton<PluginHelper>::Instance()->symbol(name, "create");
        Plugin* plugin = func();

        plugin -> set_name(name);
        
        bool is_switch = (pluginInfo.attr("switch") == "on" ? true : false);

        plugin->set_switch(is_switch);

        work->append(plugin);
    }

    return true;
}