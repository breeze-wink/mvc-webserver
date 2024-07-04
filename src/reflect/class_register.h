#pragma once

#include "class_factory.h"

namespace breeze::reflect
{

    class ClassRegister
    {
    public:
        ClassRegister(const string & class_name, create_object func)
        {
            // register class
            Singleton<ClassFactory>::Instance()->register_class(class_name, func);
        }

        ClassRegister(const string & class_name, const string & field_name, const string & field_type, uintptr_t offset)
        {
            // register class field
            Singleton<ClassFactory>::Instance()->register_class_field(class_name, field_name, field_type, offset);
        }

        ClassRegister(const string & class_name, const string & method_name, uintptr_t method)
        {
            // register class method
            Singleton<ClassFactory>::Instance()->register_class_method(class_name, method_name, method);
        }
    };


    #define REGISTER_CLASS(className)                                       \
        Object * createObject##className()                                  \
        {                                                                   \
            Object * obj = new className();                                 \
            obj->set_class_name(#className);                                \
            return obj;                                                     \
        }                                                                  	\
        ClassRegister classRegister##className(#className, createObject##className)

    #define REGISTER_CLASS_FIELD(className, fieldName, fieldType) \
        className className##fieldName; \
        ClassRegister classRegister##className##fieldName(#className, #fieldName, #fieldType, (size_t)(&(className##fieldName.fieldName)) - (size_t)(&className##fieldName))

    #define REGISTER_CLASS_METHOD(className, methodName, returnType, ...) \
        std::function<returnType(className *, ##__VA_ARGS__)> className##methodName##method = &className::methodName; \
        ClassRegister classRegister##className##methodName(#className, #methodName, (uintptr_t)&(className##methodName##method))
    
}