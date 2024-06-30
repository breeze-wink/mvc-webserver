#pragma once
#include <map>
#include <string>
#include <typeinfo>

using std::string;
using std::map;

namespace breeze::engine
{

    class Object
    {
    public:
        Object() {}
        virtual ~Object(){}
    };
    class Context
    {
    public:
        Context() = default;
        ~Context() 
        {
            clear();
        }

        template <typename T>
        T get(const string& key);

        template <typename T>
        void set(const string& key, T value);
        void set(const string& key, const char* value);
        void set(const string& key, const string& value);
        void set(const string& key, Object* value);

        void clear();

    private:
        map<string, bool>     m_bool;
        map<string, char>     m_char;
        map<string, int>      m_int ;
        map<string, double> m_double;
        map<string, string>    m_str;
        map<string, Object*>   m_obj;
    };

    template <typename T>
    T Context::get(const string& key)
    {
        if (typeid(T) == typeid(bool))
        {
            auto it = m_bool.find(key);
            if (it == m_bool.end())
            {
                return 0;
            }
            return it -> second;
        }
        else if (typeid(T) == typeid(char))
        {
            auto it = m_char.find(key);
            if (it == m_char.end())
            {
                return 0;
            }
            return it -> second;
        }
        else if (typeid(T) == typeid(int))
        {
            auto it = m_int.find(key);
            if (it == m_int.end())
            {
                return 0;
            }
            return it -> second;
        }
        else if (typeid(T) == typeid(double))
        {
            auto it = m_double.find(key);
            if (it == m_double.end())
            {
                return 0;
            }
            return it -> second;
        }
        return 0;
    }

    //模板偏特化
    template <>
    inline string Context::get<string>(const string& key)
    {
        auto it = m_str.find(key);
        if (it == m_str.end())
        {
            return "";
        }
        return it -> second;
    }

    template <>
    inline Object* Context::get<Object*>(const string& key)
    {
        auto it = m_obj.find(key);
        if (it == m_obj.end())
        {
            return nullptr;
        }
        return it -> second;
    }

    template <typename T>
    void Context::set(const string& key, T value)
    {
        if (typeid(T) == typeid(bool))
        {
            m_bool[key] = value;
        }
        else if (typeid(T) == typeid(char))
        {
            m_char[key] = value;
        }
        else if (typeid(T) == typeid(int))
        {
            m_int[key] = value;
        }
        else if (typeid(T) == typeid(double))
        {
            m_double[key] = value;
        }
    }

    inline void Context::set(const string& key, const char* value)
    {
        m_str[key] = value;
    }
    inline void Context::set(const string& key, const string& value)
    {
        m_str[key] = value;
    }

    inline void Context::set(const string& key, Object* value)
    {
        m_obj[key] = value;
    }

    inline void Context::clear()
    {
        m_bool.clear();
        m_int.clear();
        m_char.clear();
        m_double.clear();
        m_str.clear();

        for (auto obj : m_obj)
        {
            delete obj.second;
            obj.second = nullptr;
        }
        m_obj.clear();
    }

}