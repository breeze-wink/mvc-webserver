#pragma once
#include <vector>
#include <map>
#include <functional>

#include "Singleton.h"
using namespace breeze::utility;

#include "class_field.h"
#include "class_method.h"

namespace breeze::reflect
{
    
    class Object
    {
    public:
        Object() = default;
        virtual ~Object() = default;

        void set_class_name(const string & class_name);
        const string & get_class_name() const;

        int get_field_count();
        ClassField * get_field(int pos);
        ClassField * get_field(const string & field_name);

        template <typename T>
        void get(const string & field_name, T & value);

        template <typename T>
        void set(const string & field_name, const T & value);

        template <typename R = void, typename ...Args>
        R call(const string & method_name, Args&&... args);

    private:
        string m_class_name;
    };

    typedef Object * (*create_object)();

    class ClassFactory
    {
        SINGLETON(ClassFactory);
    public:
        // reflect class
        void register_class(const string & class_name, create_object func);
        Object * create_class(const string & class_name);

        // reflect class field
        void register_class_field(const string & class_name, const string & field_name, const string & field_type, size_t offset);
        int get_class_field_count(const string & class_name);
        ClassField * get_class_field(const string & class_name, int pos);
        ClassField * get_class_field(const string & class_name, const string & field_name);

        // reflect class method
        void register_class_method(const string & class_name, const string &method_name, uintptr_t method);
        int get_class_method_count(const string & class_name);
        ClassMethod * get_class_method(const string & class_name, int pos);
        ClassMethod * get_class_method(const string & class_name, const string & method_name);

        bool map_is_empty() const 
        {
            return m_class_map.empty();
        }

    private:
        std::map<string, create_object> m_class_map;
        std::map<string, std::vector<ClassField *>> m_class_fields;
        std::map<string, std::vector<ClassMethod *>> m_class_methods;
    };

    template <typename T>
    void Object::get(const string & field_name, T & value)
    {
        ClassField * field = Singleton<ClassFactory>::Instance()->get_class_field(m_class_name, field_name);
        if (field == nullptr)
        {
            return;
        }
        size_t offset = field->offset();
        value = *((T *)((unsigned char *)(this) + offset));
    }

    template <typename T>
    void Object::set(const string & field_name, const T & value)
    {
        ClassField * field = Singleton<ClassFactory>::Instance()->get_class_field(m_class_name, field_name);
        if (field == nullptr)
        {
            return;
        }
        size_t offset = field->offset();
        *((T *)((unsigned char *)(this) + offset)) = value;
    }

    template <typename R, typename ...Args>
    R Object::call(const string & method_name, Args&&... args)
    {
        ClassFactory * factory = Singleton<ClassFactory>::Instance();
        ClassMethod * method = factory->get_class_method(m_class_name, method_name);
        if (method == nullptr)
        {
            return R();
        }
        auto func = method->method();
        typedef std::function<R(decltype(this), Args...)> class_method;
        return (*((class_method *)func))(this, args...);
    }
}