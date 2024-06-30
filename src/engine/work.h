#pragma once

#include <vector>
#include "plugin.h"
#include "context.h"

namespace breeze::engine
{
    class Work
    {
    public:
        Work();
        ~Work();

        void append(Plugin* plugin);
        bool run(Context& ctx);
        void set_id(int id);
        int get_id();
        
        void set_switch(bool flag);
        bool get_switch() const;
        
    private:
        int m_id;
        bool m_switch;
        std::vector<Plugin*> m_plugins;
    };  
}