#pragma once
#include <initializer_list>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "Value.h"
using namespace breeze::utility;
using std::string;

namespace breeze::xml
{
    class Xml
    {
    public:
        Xml() = default;
        Xml(const char* name);
        Xml(const string& name);
        Xml(const Xml& other);
        ~Xml();

        string name() const;
        void set_name(const string& name);
        
        string text() const;
        void set_text(const string& text);

        Value& attr(const string& key);
        void set_attr(const string& key, const Value& value);

        /* 添加 */
        void append(const Xml& child);
        void append(const std::initializer_list<Xml> list);

        /* 删除 */
        void remove(size_t index);
        void remove(const char* name);
        void remove(const string& name);

        /* 子元素 */
        Xml& operator [](int index);
        Xml& operator [](const char* name);
        Xml& operator [](const string& name);   

        Xml& operator = (const Xml& other);

        int size() const;
        bool empty() const;
        void clear();
        
        using iterator = std::vector<Xml>::iterator;
        iterator begin()
        {
            return m_child.begin();
        }

        iterator end()
        {
            return m_child.end();
        }

        iterator erase(iterator it)
        {
            it -> clear();
            return m_child.erase(it);
        }

        string str() const;
        friend std::ostream& operator << (std::ostream& os, const Xml& xml)
        {
            os << xml.str();
            return os;
        }

        bool load(const string& filename);
        bool load(const char* buf, size_t len);
        bool save(const string& filename);

    private:
        void copy(const Xml& other);
    
    private:
        string m_name;
        string m_text;
        std::map<string, Value> m_attrs;
        std::vector<Xml> m_child;
    };    
}