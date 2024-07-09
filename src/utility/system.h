#pragma once
#include "Singleton.h"
#include <string>
using std::string;

namespace breeze::utility
{
    class System
    {
        SINGLETON(System);
    public:
        string get_root_path();
        void init();
    private:
        void core_dump();
    private:
        string m_root_path = "";
    };
}