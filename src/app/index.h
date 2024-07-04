#pragma once

#include "controller.h"

#include "request.h"
#include "response.h"

using namespace breeze::web;

namespace breeze::app
{
    class Index : public Controller
    {
    public:
        void index(const Request& req, Response& resp);
        void hello(const Request& req, Response& resp);
    };
}