#include "response.h"
#include "Logger.h"
#include "server.h"
#include "system.h"
#include <fstream>
#include <sstream>

using namespace breeze::web;
using namespace breeze::utility;

Response::Response() : m_code(200)
{
    
}
Response::~Response()
{

}

void Response::code(int code)
{
    m_code = code;
}
void Response::data(Type type, const string& data)
{
    m_type = type;
    m_data = data;
}

string Response::data() const
{
    std::ostringstream oss;
    oss << "HTTP/1.1 " << m_code << " OK\r\n";
    switch (m_type) 
    {
        case Type::HTML:
            oss << "Content-Type: text/html; charset: utf-8\r\n";
            break;
        case Type::JSON:
            oss << "Content-Type: application/json; charset: utf-8\r\n";
            break;
        case Type::JS:
            oss << "Content-Type: text/javascript; charset: utf-8\r\n";
            break;
        case Type::CSS:
            oss << "Content-Type: text/css; charset: utf-8\r\n";
            break;
        case Type::JPG:
            oss << "Content-Type: image/jpeg; charset: utf-8\r\n";
            break;
        case Type::PNG:
            oss << "Content-Type: image/png; charset: utf-8\r\n";
            break;
        case Type::GIF:
            oss << "Content-Type: image/gif; charset: utf-8\r\n";
            break;
        case Type::ICO:
            oss << "Content-Type: image/x-icon; charset: utf-8\r\n";
            break;
        default:
            break;
    }   
    oss << "Content-Length: " << m_data.size() << "\r\n\r\n";
    oss << m_data << "\r\n";
    return oss.str();
}

void Response::html(const string& data)
{
    m_type = Type::HTML;
    m_data = data;
}
void Response::json(const string& data)
{
    m_type = Type::JSON;
    m_data = data;
}

string Response::page_not_found()
{
    auto sys = Singleton<System>::Instance();
    string file_path = sys -> get_root_path() + "/template/404.html";
    std::ifstream in(file_path);
    
    std::ostringstream content;
    content << in.rdbuf();
    string data = content.str();
    
    std::ostringstream response;

    response << "HTTP/1.1 404 Not Found\r\n";
    response << "Content-Type: text/html; charset: utf-8\r\n";
    response << "Content-Length: " << data.size() << "\r\n\r\n";
    response << data << "\r\n";
    

    return response.str();
}

void Response::render(const string& file_name)
{
    const string& template_path = Singleton<Server>::Instance() ->get_template_folder();
    const string& file_path = template_path + "/" + file_name;

    log_debug("template path : %s", file_path.c_str());

    std::ifstream ifs(file_path);

    if (ifs.fail())
    {
        return;
    }
    
    std::ostringstream oss;
    oss << ifs.rdbuf();
    m_type = Type::HTML;
    m_data = oss.str();
}