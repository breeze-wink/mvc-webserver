#include "server.h"
#include "../frame/server.h"
#include "response.h"
#include "String.h"
#include "system.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace breeze;
using namespace breeze::web;

void Server::start()
{
    try
    {
        Singleton<frame::Server>::Instance() -> start();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}


void Server::bind(const string& path, server_handler handler)
{
    m_handlers[path] = handler;
}

string Server::handle(const Request& req)
{
    const string& path = req.path();

    auto it = m_handlers.find(path);

    if (it != m_handlers.end())
    {
        Response resp;
        it -> second(req, resp);
        return resp.data();
    }

    using Type = Response::Type;

    auto func = [](const string& file_path, Response::Type type){
        std::ifstream ifs(file_path);

        std::ostringstream oss;
        oss << ifs.rdbuf();
        ifs.close();

        Response resp;
        resp.data(type, oss.str());

        return resp.data();
    };
    
    if (String::hasSuffix(path, ".html"))
    {
        const string& file_path = get_template_folder() + path;

        return func(file_path, Type::HTML);
    }
    else if (String::hasSuffix(path, ".js"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::JS);
    }
    else if (String::hasSuffix(path, ".css"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::CSS);
    }
    else if (String::hasSuffix(path, ".jpg"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::JPG);
    }
    else if (String::hasSuffix(path, ".png"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::PNG);
    }
    else if (String::hasSuffix(path, ".gif"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::GIF);
    }
    else if (String::hasSuffix(path, ".ico"))
    {
        const string& file_path = get_static_folder() + path;

        return func(file_path, Type::ICO);
    }
    return Response::page_not_found();
}

void Server::set_template_folder(const string& template_folder)
{
    m_template_folder = template_folder;
}

string Server::get_template_folder() const
{
    const string& root_path = Singleton<System>::Instance() -> get_root_path();
    string folder_path = root_path + "template";
    return folder_path;
}

void Server::set_static_folder(const string& static_folder)
{
    m_static_folder = static_folder;
}
string Server::get_static_folder()const
{
    const string& root_path = Singleton<System>::Instance() -> get_root_path();
    string folder_path = root_path + "static";
    return folder_path;
}