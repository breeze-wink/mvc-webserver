#include "server.h"
// #include "request.h"
// #include "response.h"


using namespace breeze::web;

void post(const Request& req, Response& resp)
{
    string name = req.post("name");
    int age = req.post("age");

    Json json;
    json["name"] = name;
    json["age"]  = age;

    resp.json(json.str());
}


int main()
{
    auto server = Singleton<Server>::Instance();

    //静态绑定
    server_handler post_handler = post;
    // server_handler index_handler = index;

    server -> bind("/post", post_handler);
    // server -> bind("/", index_handler);
    server -> start();

    return 0;
}