#pragma once
#include "plugin.h"

using namespace breeze::engine;

namespace breeze::plugin
{
    class EchoPlugin : public Plugin
    {
    public:
        EchoPlugin();
        ~EchoPlugin();

        
        virtual bool run(Context& ctx) override;
    };
    
    DECLARE_PLUGIN(EchoPlugin)
}

