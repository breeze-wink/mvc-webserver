#pragma once
#include "Singleton.h"
#include "xml.h"
#include "work.h"
#include <map>

using namespace breeze::utility;
using namespace breeze::xml;

namespace breeze::engine
{
    class Workflow
    {
        SINGLETON(Workflow);
        
    public:
        bool load(const string& config);
        bool run (int id, const string& input, string& output);

    private:
        bool load_plugin(Work* work, Xml& elem);
        std::map<int, Work*> m_works;
    };
}
