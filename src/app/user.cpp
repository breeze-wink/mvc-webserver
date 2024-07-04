#include "user.h"


using namespace breeze::app;

CONTROLLER(User);
ACTION(User, index);

void User::index(const Request& req, Response& resp)
{
    resp.render("user/index.html");
}
