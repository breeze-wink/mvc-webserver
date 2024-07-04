#pragma once

#include <string>
using std::string;

namespace breeze::reflect
{
    class ClassField
    {
    public:
        ClassField() : m_name(""), m_type(""), m_offset(0) {}
        ClassField(const string & name, const string & type, size_t offset) : m_name(name), m_type(type), m_offset(offset) {}
        ~ClassField() = default;

        const string & name() const
        {
            return m_name;
        }

        const string & type() const
        {
            return m_type;
        }

        size_t offset() const
        {
            return m_offset;
        }

    private:
        string m_name;
        string m_type;
        size_t m_offset;
    };

}