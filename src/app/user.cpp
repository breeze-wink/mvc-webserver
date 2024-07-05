#include "user.h"


using namespace breeze::app;

CONTROLLER(User);
ACTION(User, index);
ACTION(User, tryall);

void User::index(const Request& req, Response& resp)
{
    resp.render("user/index.html");
}

void User::tryall(const Request& req, Response& resp)
{
    resp.render("user/all.html");
}
