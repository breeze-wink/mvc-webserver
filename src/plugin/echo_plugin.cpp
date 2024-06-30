#include "echo_plugin.h"
#include "Logger.h"
#include "plugin.h"
#include "system.h"

using namespace breeze::plugin;
using namespace breeze::utility;


EchoPlugin::EchoPlugin()
{
    auto sys = Singleton<System>::Instance();
    const string root_path = sys -> get_root_path();
    auto logger = Singleton<Logger>::Instance();
    string log_path = root_path + "log/echo_plugin.log";
    logger -> open(log_path);
    logger -> setConsole(false);
}
EchoPlugin::~EchoPlugin()
{

}

bool EchoPlugin::run(Context& ctx)
{
    string input = ctx.get<string>("input");

    log_info("echo plugin running!");
    log_info("context input: %s", input.c_str());

    ctx.set("output", "echo_plugin run: " + input);
    return true;
}

DEFINE_PLUGIN(EchoPlugin)