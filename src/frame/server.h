#include "Singleton.h"
#include <string>

using namespace breeze::utility;
using std::string;

namespace breeze::frame
{
    class Server
    {
        SINGLETON(Server);
    public:
        void start();

    private:
        string m_ip = "";
        int m_port = 0;
        int m_threads = 0;
        int m_max_conns = 0;
        int m_wait_time = 0;
        int m_log_level = 0;
    };
}