#pragma once

#include <string>
using std::string;

namespace breeze::reflect
{

    class ClassMethod
    {
    public:
        ClassMethod() : m_name(""), m_method(0) {}
        ClassMethod(const string & name, uintptr_t method) : m_name(name), m_method(method) {}
        ~ClassMethod() = default;

        const string & name() const
        {
            return m_name;
        }

        uintptr_t method() const
        {
            return m_method;
        }

    private:
        string m_name;
        uintptr_t m_method;
    };
    
}