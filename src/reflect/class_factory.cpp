#include "class_factory.h"

using namespace breeze::reflect;

void Object::set_class_name(const string & class_name)
{
    m_class_name = class_name;
}

const string & Object::get_class_name() const
{
    return m_class_name;
}

int Object::get_field_count()
{
    return Singleton<ClassFactory>::Instance()->get_class_field_count(m_class_name);
}

ClassField * Object::get_field(int pos)
{
    return Singleton<ClassFactory>::Instance()->get_class_field(m_class_name, pos);
}

ClassField * Object::get_field(const string & field_name)
{
    return Singleton<ClassFactory>::Instance()->get_class_field(m_class_name, field_name);
}

void ClassFactory::register_class(const string & class_name, create_object func)
{
    m_class_map[class_name] = func;
}

Object * ClassFactory::create_class(const string & class_name)
{

    auto it = m_class_map.find(class_name);
    if (it == m_class_map.end())
    {
        return nullptr;
    }
    return it->second();
}

void ClassFactory::register_class_field(const string & class_name, const string & field_name, const string & field_type, size_t offset)
{
    m_class_fields[class_name].push_back(new ClassField(field_name, field_type, offset));
}

int ClassFactory::get_class_field_count(const string & class_name)
{
    return (int)m_class_fields[class_name].size();
}

ClassField * ClassFactory::get_class_field(const string & class_name, int pos)
{
    int size = (int)m_class_fields[class_name].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_class_fields[class_name][pos];
}

ClassField * ClassFactory::get_class_field(const string & class_name, const string & field_name)
{
    auto & fields = m_class_fields[class_name];
    for (auto it = fields.begin(); it != fields.end(); ++it)
    {
        if ((*it)->name() == field_name)
        {
            return *it;
        }
    }
    return nullptr;
}

void ClassFactory::register_class_method(const string & class_name, const string &method_name, uintptr_t method)
{
    m_class_methods[class_name].push_back(new ClassMethod(method_name, method));
}

int ClassFactory::get_class_method_count(const string & class_name)
{
    return (int)m_class_methods[class_name].size();
}

ClassMethod * ClassFactory::get_class_method(const string & class_name, int pos)
{
    int size = (int)m_class_methods[class_name].size();
    if (pos < 0 || pos >= size)
    {
        return nullptr;
    }
    return m_class_methods[class_name][pos];
}

ClassMethod * ClassFactory::get_class_method(const string & class_name, const string & method_name)
{
    auto & methods = m_class_methods[class_name];
    for (auto it = methods.begin(); it != methods.end(); ++it)
    {
        if ((*it)->name() == method_name)
        {
            return *it;
        }
    }
    return nullptr;
}
