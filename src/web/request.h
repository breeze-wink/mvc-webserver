#pragma once
#include <map>
#include <string>
#include "Json.h"
#include "file_upload.h"

using namespace breeze::json;

using std::string;

namespace breeze::web
{
    class Request
    {
    public:
        Request() = default;
        ~Request() = default;

        int parse_header(const char* buf, int len);
        int parse_body(const char* buf, int len);

        bool is_get() const;
        bool is_post() const;
        bool is_delete() const;

        string get(const string& name) const;
        Json post(const string& name = "") const;

        FileUpload file(const string& name) const;
        string header(const string& name) const;
        string cookie(const string& name) const;
        string path() const;
        string user_agent() const;
        string user_host() const;

        int content_length() const;
        void show() const;

    private:
        string m_method;
        string m_uri;
        string m_protocol;
        string m_path;
        string m_body;
        string m_query_string;
        std::map<string, string> m_gets;
        Json m_post;
        std::map<string, string> m_headers;
        std::map<string, string> m_cookies;
        std::map<string, FileUpload> m_files;
    };
}