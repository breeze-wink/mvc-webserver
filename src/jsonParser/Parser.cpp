#include "Parser.h"
#include <fstream>
#include <sstream>
using namespace breeze::json;

void Parser::load(const string &fileName) {
    std::ifstream ifs(fileName);
    std::ostringstream oss;
    oss << ifs.rdbuf();
    m_str = oss.str();
    m_idx = 0;
}

void Parser::load(const char *buf, int len) {
    m_str.assign(buf, len);
    m_idx = 0;
}

Json Parser::parse() {
    char ch = get_next_token();
    switch (ch) {
        case 'n':
            //null
            m_idx --;
            return parse_null();
        case 't':
        case 'f':
            m_idx --;
            return parse_bool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            // number
            m_idx --;
            return parse_number();
        case '"':
            //"string"
            return Json(parse_string());
        case '[':
            //array
            return parse_array();
        case '{':
            //object
            return parse_object();
        default:
            break;
    }
    throw std::logic_error("unexpected character");
}

void Parser::skip_white_space() {
    while(m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
    {
        m_idx ++;
    }
}

char Parser::get_next_token() {
    skip_white_space();
    if(m_idx == m_str.size())
    {
        throw std::logic_error("ilegal operation");
    }
    return m_str[m_idx ++];
}

Json Parser::parse_null() {
    if(m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }

    throw std::logic_error("parse null error");
}

Json Parser::parse_bool() {
    if(m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if(m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }

    throw std::logic_error("parse bool error");
}

Json Parser::parse_number() {
    size_t pos = m_idx;
    if (m_str[m_idx] == '-')
    {
        m_idx ++;
    }
    if(m_str[m_idx] == '0')
    {
        m_idx ++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx ++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx ++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }
    if(m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));
    }
    m_idx ++;
    if(!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("invalid float number");
    }
    while(in_range(m_str[m_idx], '0', '9'))
    {
        m_idx ++;
    }
    return Json(std::atof(m_str.c_str() + pos));
}

Json Parser::parse_string() {
    int pos = m_idx;
    while(true)
    {
        char ch = get_next_token();
        if (ch == '"')
        {
            break;
        }
        if (ch == '\\')
        {
            ch = get_next_token();
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '\\':
                    break;
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }
    return Json(m_str.substr(pos, m_idx - pos - 1));
}

Json Parser::parse_array() {
    Json arr(Json::JSON_ARRAY);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx --;
    while(true)
    {
        arr.append(parse());//递归
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if(ch != ',')
        {
            throw std::logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object() {
    Json obj(Json::JSON_OBJECT);
    char ch = get_next_token();
    if(ch == '}')
    {
        return obj;
    }
    m_idx --;
    while(true)
    {
        ch = get_next_token();
        if(ch != '"')
        {
            throw std::logic_error("invalid object key");
        }
        string key = parse_string();
        ch = get_next_token();
        if(ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse();
        ch = get_next_token();
        if(ch == '}')
        {
            break;
        }
        if(ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}

bool Parser::in_range(int x, int lower, int upper) const
{
    return x >= lower && x <= upper;
}
