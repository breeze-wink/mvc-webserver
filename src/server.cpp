#include "server.h"

using namespace breeze::frame;

int main()
{
    auto server = Singleton<Server>::Instance();

    server -> start();

    return 0;
}