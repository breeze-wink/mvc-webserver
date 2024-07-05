#pragma once
#include "controller.h"
#include "request.h"
#include "response.h"

using namespace breeze::web;

namespace breeze::app
{
    class User : public Controller
    {
    public:
        void index(const Request& req, Response& resp);
        void tryall(const Request& req, Response& resp);
        void happy(const Request& req, Response& resp);
    };
}