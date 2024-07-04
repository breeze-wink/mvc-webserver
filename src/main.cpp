#include "server.h"
// #include "request.h"
// #include "response.h"


using namespace breeze::web;


int main()
{
    auto server = Singleton<Server>::Instance();

    //静态绑定
    // server_handler hello_handler = hello;
    // server_handler index_handler = index;

    // server -> bind("/hello", hello_handler);
    // server -> bind("/", index_handler);
    server -> start();

    return 0;
}