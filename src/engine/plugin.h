#pragma once
#include <string>
#include "context.h"
using std::string;

namespace breeze::engine
{
    class Plugin
    {
    public:
        Plugin() : m_switch(false) {};
        virtual ~Plugin() = default;

        virtual bool run(Context& ctx) = 0;

        void set_name(const string& name)
        {
            m_name = name;
        }
        const string& get_name()
        {
            return m_name;
        }
        void set_switch(bool flag)
        {
            m_switch = flag;
        }
        const bool get_switch()
        {
            return m_switch;
        }
    private:
        string m_name;
        bool m_switch;
    };

#define DECLARE_PLUGIN(className)               \
    extern "C" Plugin * create();               \
    extern "C" void    destory();               \

#define DEFINE_PLUGIN(className)                \
    extern "C" Plugin * create()                \
    {                                           \
        return new (std::nothrow) className();  \
    }                                           \
    extern "C" void destroy(Plugin* p)          \
    {                                           \
        delete p;                               \
        p = nullptr;                            \
    }
}