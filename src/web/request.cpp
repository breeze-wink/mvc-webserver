#include "request.h"
#include "response.h"
#include <cstring>
#include <string>
#include <vector>
#include "String.h"
#include "Logger.h"

using namespace breeze::web;
using namespace breeze::utility;

int Request::parse_header(const char* buf, int len)
{
    /* 解析method uri protocol*/
    const char* start = buf;
    const char* end   = buf + len - 1;
    const char* i = start;

    auto skip_chars = [&](const string& str){
        while (i < end && str.find(*i) != string::npos) i++;
    };

    auto go_to_next_char = [&](const string str){
        while (i < end && str.find(*i) == string::npos) i ++;
    };


    //解析method
    skip_chars(" ");

    start = i; //定位起点
    go_to_next_char(" ");

    m_method = string(start, 0, i - start);

    //解析 uri
    skip_chars(" ");
    start = i;
    go_to_next_char(" ");
    m_uri = string(start, 0, i - start);

    //解析protocol
    skip_chars(" ");
    start = i;
    go_to_next_char("\r\n");
    m_protocol = string(start, 0, i - start);

    skip_chars("\r\n");

    //如果uri 存在 ？ 需要进一步处理

    auto pos = m_uri.find('?');
    if (pos != string::npos)
    {
        m_path = m_uri.substr(0, pos);
        m_query_string = m_uri.substr(pos + 1);
        std::vector<string> querys = String::split(m_query_string, '&');
        for (auto query : querys)
        {
            auto split_pos = query.find('=');
            string left = query.substr(0, split_pos);
            string right = split_pos < query.size() - 1 ? query.substr(split_pos + 1) : "";
            m_gets[left] = right;
        }
    }
    else
    {
        m_path = m_uri;
    }

    //解析header
    while (i < end)
    {
        start = i;
        go_to_next_char(": ");
        string name = string(start, 0, i - start);

        skip_chars(": ");
        start = i;
        go_to_next_char("\r\n");
        string value = string(start, 0, i - start);

        m_headers[name] = value;

        //结束
        if (strncmp(i, "\r\n\r\n", 4) == 0)
        {
            i += 4;
            break;
        }

        skip_chars("\r\n");
    }
    return i - buf;
}

int Request::parse_body(const char* buf, int len)
{
    //TODO
    return 0;
}


bool Request::is_get() const
{
    return m_method == "GET";
}
bool Request::is_post() const
{
    return m_method == "POST";
}
bool Request::is_delete() const
{
    return m_method == "DELETE";
}

string Request::get(const string& name) const
{
    auto it = m_gets.find(name);
    if (it == m_gets.end())
    {
        return "";
    }
    return it -> second;
}

Json Request::post(const string& name) const
{
    if (name.empty())
    {
        return m_post;
    }

    return m_post.get(name);
}


string Request::header(const string& name) const
{
    auto it = m_headers.find(name);
    if (it == m_headers.end())
    {
        return "";
    }
    return it -> second;
}

string Request::cookie(const string& name) const
{
    auto it = m_cookies.find(name);
    if (it == m_cookies.end())
    {
        return "";
    }

    return it -> second;
}
string Request::path() const
{
    return m_path;
}
string Request::user_agent() const
{
    return header("User-Agent");
}
string Request::user_host() const
{
    return header("Host");
}

int Request::content_length() const
{
    return std::stoi(header("Content_Length"));
}

void Request::show() const
{
    log_debug("http method: %s", m_method.c_str());
    log_debug("http uri: %s", m_uri.c_str());
    log_debug("http protocol: %s", m_protocol.c_str());
    log_debug("http path: %s", m_path.c_str());
    log_debug("http query string: %s", m_query_string.c_str());
    log_debug("http headers <<---");
    for (auto header : m_headers)
    {
        log_debug("%s: %s", header.first.c_str(),header.second.c_str());
    }
    log_debug("--->>");
    log_debug("http get paraments <<---");
    for (auto get : m_gets)
    {
        log_debug("%s = %s", get.first.c_str(), get.second.c_str());
    }
    log_debug("---->>");

    log_debug("http post paraments <<---");
    log_debug("%s", m_post.str().c_str());
    log_debug("---->>");

    log_debug("http body: \n%s", m_body.c_str());
}