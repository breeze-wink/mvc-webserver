#include "server.h"
#include "Singleton.h"
#include "system.h"
#include "iniFile.h"
#include "Logger.h"
#include <csignal>
#include "task_dispatcher.h"
#include "socket_handler.h"

using namespace breeze::frame;
using namespace breeze::socket;
using namespace breeze::thread;

void Server::signal_handle()
{
    //使用sigaction进行服务器需要的信号处理
    struct sigaction sig_action;
    sig_action.sa_handler = [](int signo) {
        switch (signo) {
        case SIGINT:
        case SIGTERM:
            log_debug("ready to finish");
            Singleton<SocketHandler>::Instance() -> stop();
            Singleton<TaskDispatcher>::Instance() -> stop();
            break;
        default:
            break;
        }
    };
    sigemptyset(&sig_action.sa_mask);
    if (sigaction(SIGINT, &sig_action, nullptr) < 0)
    {
        log_error("sigaction error");
    }
    if (sigaction(SIGTERM, &sig_action, nullptr) < 0)
    {
        log_error("sigaction error");
    }

    struct sigaction SIGPIPE_action;
    SIGPIPE_action.sa_handler = SIG_IGN;
    sigemptyset(&SIGPIPE_action.sa_mask);
    if (sigaction(SIGPIPE, &SIGPIPE_action, nullptr) < 0)
    {
        log_error("sigaction error");
    }

}


void Server::start()
{
    signal_handle();

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
    logger -> setMaxSize(1024 * 1024);

    auto dispatcher = Singleton<TaskDispatcher>::Instance();
    dispatcher -> init(m_threads);

    auto handle = Singleton<SocketHandler>::Instance();
    handle -> listen(m_ip, m_port);
    handle -> handle(m_max_conns, m_wait_time);

    dispatcher -> join();

    log_info("Server finish formally");
}
