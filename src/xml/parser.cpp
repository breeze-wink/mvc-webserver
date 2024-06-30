#include "parser.h"
#include <cctype>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <stdexcept>
using namespace breeze::xml;

bool Parser::load(const string& filename)
{
    std::ifstream in(filename);
    if (in.fail())
    {
        return false;
    }
    std::ostringstream oss;
    oss << in.rdbuf();

    m_str = oss.str();
    m_idx = 0;

    return true;
}   
bool Parser::load(const char* buf, size_t len)
{
    m_str.assign(buf, len);
    m_idx = 0;
    return true;
}
Xml Parser::parse()
{
    skip_white_space();
    if (m_str.compare(m_idx, 5, "<?xml") == 0)
    {
        if (!parse_declaration())
        {
            throw std::logic_error("parse declaration error");
        }
    }

    skip_white_space();
    while (m_str.compare(m_idx, 4, "<!--") == 0)
    {
        if (!parse_comment())
        {
            throw std::logic_error("parse comment error");
        }
        skip_white_space();
    }

    // 元素判断
    if (m_str[m_idx] == '<' && (std::isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_'))
    {
        return parse_element();
    }

    throw std::logic_error("parse element error");
}

void Parser::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t' || m_str[m_idx] == '\r')
    {
        m_idx ++;
    }
}
bool Parser::parse_declaration()
{
    if (m_str.compare(m_idx, 5, "<?xml") != 0)
    {
        return false;
    }
    m_idx += 5;

    size_t pos = m_str.find("?>", m_idx);

    if (pos == string::npos)
    {
        return false;
    }
    m_idx = pos + 2;
    return true;
}
bool Parser::parse_comment()
{
    if (m_str.compare(m_idx, 4, "<!--") != 0)
    {
        return false;
    }

    m_idx += 4;
    size_t pos = m_str.find("-->", m_idx);

    if (pos == string::npos)
    {
        return false;
    }
    m_idx = pos + 3;
    return true;
}   
Xml Parser::parse_element()
{
    Xml elem;
    m_idx ++;
    skip_white_space();
    auto name = parse_element_name();
    elem.set_name(name);

    while (m_str[m_idx] != '\0')
    {
        skip_white_space();
        if (m_str[m_idx] == '/') // 元素为空
        {
            if (m_str[m_idx + 1] == '>')
            {
                m_idx += 2;
                break;
            }
            else
            {
                throw std::logic_error("parse empty element error");
            }
        }
        else if (m_str[m_idx] == '>')
        {
            m_idx ++;
            auto text = parse_element_text();
            if (! text.empty())
            {
                elem.set_text(text);
            }
        }
        else if (m_str[m_idx] == '<')
        {
            if (m_str[m_idx + 1] == '/') // 闭合标签
            {
                string end_tag = "</" + name + ">";
                size_t pos = m_str.find(end_tag, m_idx);

                if (pos == string::npos)
                {
                    throw std::logic_error("Failed to find end tag: " + name);
                }
                m_idx = pos + end_tag.size();
                break;
            }
            else if (m_str.compare(m_idx, 4, "<!--") == 0)
            {
                if (!parse_comment())
                {
                    throw std::logic_error("Failed to parse comment");
                }
            }
            else //有孩子节点
            {
                elem.append(parse_element());
            }
        }
        else // attr
        {
            string key = parse_element_attr_key();
            skip_white_space();
            if (m_str[m_idx] != '=')
            {
                throw std::logic_error("Failed to parse attr: " + key);
            }
            m_idx ++;

            skip_white_space();
            string val = parse_element_atrr_val();
            elem.set_attr(key, val);
        }
        
    }
    return elem;
}
string Parser::parse_element_name()
{
    size_t pos = m_idx;
    if (isalpha(m_str[m_idx]) || m_str[m_idx] == '_')
    {
        m_idx ++;
        while (isalpha(m_str[m_idx])||
               m_str[m_idx] == '_'  ||
               m_str[m_idx] == '-'  ||
               m_str[m_idx] == '.')
        {
            m_idx ++;
        }
    }
    return m_str.substr(pos, m_idx - pos);
}
string Parser::parse_element_text()
{
    size_t pos = m_idx;
    while (m_str[m_idx] != '<')
    {
        m_idx ++;
    }

    return m_str.substr(pos, m_idx - pos);
}
string Parser::parse_element_attr_key()
{
    size_t pos = m_idx;
    if (isalpha(m_str[m_idx]) || m_str[m_idx] == '_')
    {
        m_idx ++;
        while (isalpha(m_str[m_idx])||
               m_str[m_idx] == '_'  ||
               m_str[m_idx] == '-'  ||
               m_str[m_idx] == '.')
        {
            m_idx ++;
        }
    }
    return m_str.substr(pos, m_idx - pos);
}
string Parser::parse_element_atrr_val()
{
    if (m_str[m_idx] != '"')
    {
        throw std::logic_error("Failed to parse attr value");
    }
    m_idx ++;
    size_t pos = m_idx;
    while (m_str[m_idx] != '"')
    {
        m_idx ++;
    }
    m_idx ++;
    return m_str.substr(pos, m_idx - pos - 1);
}