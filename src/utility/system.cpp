#include "system.h"
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/resource.h>
using namespace breeze::utility;

void System::init()
{
    core_dump();
    m_root_path = get_root_path();
    
    const string& log_path = m_root_path + "log";
    DIR* dirent = opendir(log_path.c_str());

    if (dirent == nullptr)
    {
        mkdir(log_path.c_str(), 0755);
    }
    else
    {
        closedir(dirent);
    }
}

string System::get_root_path()
{
    if (!m_root_path.empty())
    {
        return m_root_path;
    }

    char path[1024];
    int len = readlink("/proc/self/exe", path, sizeof(path));

    if (len < 0 || len > sizeof(path))
    {
        return "";
    }

    string ret = path;
    auto pos = ret.rfind("bin");
    ret.erase(pos);

    m_root_path = ret;
    return m_root_path;    
}

void System::core_dump()
{
    struct rlimit x;
    x.rlim_cur = RLIM_INFINITY;
    x.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &x);

}