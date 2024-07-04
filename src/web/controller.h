#pragma once
#include "class_factory.h"
#include "class_register.h"

using namespace breeze::reflect;

namespace breeze::web
{
    class Controller: public Object
    {
    public:
        Controller(){}
        virtual ~Controller() {}
    };

#define CONTROLLER(classname) \
        REGISTER_CLASS(classname)

#define ACTION(classname, method)\
        REGISTER_CLASS_METHOD(classname, method, void, const Request&, Response&)
}