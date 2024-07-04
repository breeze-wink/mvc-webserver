#include "server.h"
#include "request.h"
#include "response.h"


using namespace breeze::web;

void hello(const Request& req, Response& resp)
{
    string name = req.get("name");
    string age  = req.get("age");
    resp.html("hello, name = " + name + ", age = " + age);
}

void index(const Request& req, Response& resp)
{
    resp.render("home.html");
}

int main()
{
    auto server = Singleton<Server>::Instance();

    server_handler hello_handler = hello;
    server_handler index_handler = index;

    server -> bind("/hello", hello_handler);
    server -> bind("/", index_handler);
    server -> start();

    return 0;
}