#pragma once

#include "controller.h"

#include "request.h"
#include "response.h"

using namespace breeze::web;

namespace breeze::app
{
    class File : public Controller
    {
    public:
        void index(const Request& req, Response& resp);
        void upload(const Request& req, Response& resp);
    };
}