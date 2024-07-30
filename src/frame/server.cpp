#include "server.h"
#include "system.h"
#include "iniFile.h"
#include "Logger.h"
#include "task_dispatcher.h"
#include "socket_handler.h"

using namespace breeze::frame;
using namespace breeze::socket;
using namespace breeze::thread;

void Server::start()
{
    auto sys = Singleton<System>::Instance();
    sys -> init();

    string root_path = sys -> get_root_path();
    string config_file_path = root_path + "config/config.ini";
    string server_log_path = root_path + "log/server.log";

    auto ini = Singleton<IniFile>::Instance();
    ini -> load(config_file_path);

    m_ip = (string)ini -> get("server", "ip");
    m_port = ini -> get("server", "port");
    m_threads = ini -> get("server", "threads");
    m_max_conns = ini -> get("server", "max_conns");
    m_wait_time = ini -> get("server", "wait_time");
    m_log_level = ini -> get("server", "log_level");

    auto logger = Singleton<Logger>::Instance();
    logger -> open(server_log_path);
    logger -> setConsole(false);
    logger -> setLevel(static_cast<Logger::Level>(m_log_level));

    auto dispatcher = Singleton<TaskDispatcher>::Instance();
    dispatcher -> init(m_threads);

    auto handle = Singleton<SocketHandler>::Instance();
    handle -> listen(m_ip, m_port);
    handle -> handle(m_max_conns, m_wait_time);
}