#include "server.h"
#include "../frame/server.h"
#include "Logger.h"
#include "class_factory.h"
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

    auto bin_func = [](const string& file_path, Response::Type type){
        std::ifstream ifs(file_path, std::ios::binary);

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

        return bin_func(file_path, Type::JPG);
    }
    else if (String::hasSuffix(path, ".png"))
    {
        const string& file_path = get_static_folder() + path;

        return bin_func(file_path, Type::PNG);
    }
    else if (String::hasSuffix(path, ".gif"))
    {
        const string& file_path = get_static_folder() + path;

        return bin_func(file_path, Type::GIF);
    }
    else if (String::hasSuffix(path, ".ico"))
    {
        const string& file_path = get_static_folder() + path;

        return bin_func(file_path, Type::ICO);
    }

    log_debug("enter controller auto");

    string classname;
    string methodname;

    auto arr = String::split(String::trim(path, " /"), '/');

    log_debug("path = %s", path.c_str());

    if (arr.size() == 0) //默认
    {
        
        classname = "Index";
        methodname = "index";
    }
    else if (arr.size() == 1)
    {
        classname = String::capitalize(arr[0]);
        methodname = "index";
    }

    else if (arr.size() == 2) // 一个控制器，一个方法
    {
        classname = String::capitalize(arr[0]);
        methodname = arr[1];
    }

    // log_debug("classname = %s, methodname = %s", classname.c_str(), methodname.c_str());

    auto factory = Singleton<reflect::ClassFactory>::Instance();

    if (factory -> map_is_empty())
    {
        log_error("something wrong happens during reflecting, class_map is empty");
    }

    auto ctrl = factory -> create_class(classname);

    if (ctrl == nullptr)
    {
        log_error("Failed to get controller, name: %s", classname.c_str());
        return Response::page_not_found();
    }

    auto method = factory -> get_class_method(classname, methodname);
    if (method == nullptr)
    {
        log_error("Failed to get method, controller name: %s, method name: %s", classname.c_str(), methodname.c_str());
        delete ctrl;
        return Response::page_not_found();
    }

    log_debug("Succeed getting controller and method");

    try
    {
        Response resp;
        ctrl -> call(methodname, req, resp);
        delete ctrl;
        return resp.data();
    }
    
    catch (const std::exception& e)
    {
        if (ctrl)
        {
            delete ctrl;
        }
        Response resp;
        resp.code(404);
        resp.html(e.what());

        return resp.data();
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