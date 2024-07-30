#include "index.h"

using namespace breeze::app;

CONTROLLER(Index);
ACTION(Index, index);
ACTION(Index, hello);


void Index::index(const Request& req, Response& resp)
{
    resp.render("index/index.html");
}

void Index::hello(const Request& req, Response& resp)
{
    resp.render("index/hello.html");
}