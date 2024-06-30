#include "xml.h"
#include "parser.h"
#include <sstream>
#include <fstream>
using namespace breeze::xml;

Xml::Xml(const char* name) : m_name(name){}
Xml::Xml(const string& name):m_name(name){}
Xml::Xml(const Xml& other)
{
    copy(other);
}

Xml& Xml::operator = (const Xml& other)
{   
    if (this == &other)
    {
        return *this;
    }
    copy(other);
    return *this;
}


Xml::~Xml()
{
    clear();
}


string Xml::name() const
{
    return m_name;
}

void Xml::set_name(const string& name)
{
    m_name = name;
}

string Xml::text() const
{
    return m_text;
}
void Xml::set_text(const string& text)
{
    m_text = text;
}

Value& Xml::attr(const string& key)
{
    return m_attrs.at(key);
}
void Xml::set_attr(const string& key, const Value& value)
{
    m_attrs[key] = value;
}

void Xml::append(const Xml& child)
{
    m_child.push_back(child);
}

void Xml::append(const std::initializer_list<Xml> list)
{
    m_child.insert(m_child.end(), list);
}

void Xml::remove(size_t index)
{
    if (m_child.empty())
    {
        return;
    }
    if (index >= m_child.size())
    {
        return;
    }

    m_child.erase(m_child.begin() + index);
}

void Xml::remove(const char* name) // 删除所以匹配key的
{
    for (auto it = m_child.begin(); it != m_child.end();)
    {
        if (it -> m_name == name)
        {
            it = m_child.erase(it);
        }
        else
        {
            it ++;
        }
    }
}
void Xml::remove(const string& name)
{
    remove(name.c_str());
}


Xml& Xml::operator [](int index)
{
    return m_child.at(index);
}
Xml& Xml::operator [](const char* name)
{
    for (auto it = m_child.begin(); it != m_child.end(); ++ it)
    {
        if (it -> m_name == name)
        {
            return *it;
        }
    }
    
    m_child.push_back(Xml(name));
    return m_child.back();
}

Xml& Xml::operator [](const string& name)
{
    return (*this)[name.c_str()];
}

int Xml::size() const
{
    return m_child.size();
}

bool Xml::empty() const
{
    return m_child.empty();
}

void Xml::clear()
{
    m_name.clear();
    m_text.clear();
    m_attrs.clear();
    m_child.clear();
}

string Xml::str() const
{
    if (m_name.empty())
    {
        return "";
    }

    std::ostringstream oss;
    oss << "<";
    oss << m_name;
    for (auto it = m_attrs.begin(); it != m_attrs.end(); ++it)
    {
        oss << " " << it -> first << "=" << "\"" << (string)(it -> second) << "\"";
    }
    oss << ">";

    oss << m_text;
    for (auto it = m_child.begin(); it != m_child.end(); ++ it)
    {
        oss << it-> str();
    }
    
    oss << "</" << m_name << ">";

    return oss.str();
}

void Xml::copy(const Xml& other)
{
    clear();
    m_name = other.m_name;
    m_text = other.m_text;

    m_attrs = other.m_attrs;
    m_child = other.m_child;
}

bool Xml::load(const string& filename)
{
    Parser p;
    if (!p.load(filename))
    {
        return false;
    }
    *this = p.parse();
    return true;
}
bool Xml::load(const char* buf, size_t len)
{
    Parser p;
    if (!p.load(buf, len))
    {
        return false;
    }
    *this = p.parse();
    return true;
}
bool Xml::save(const string& filename)
{
    static const string root = "record/";

    string path = root + filename;

    std::ofstream out(path);

    if (out.fail())
    {
        return false;
    }

    out << str();
    out.close();

    return true;
}
