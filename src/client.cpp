#include "socket.h"
#include "system.h"
#include "work_task.h"
#include <cstdint>
#include <cstdio>
#include <cstring>
using namespace breeze::socket;
using namespace breeze::task;
int main()
{
    auto sys = Singleton<System>::Instance();

    string root_path = sys -> get_root_path();
    string client_log_path = root_path + "log/client.log";
    Singleton<Logger>::Instance() -> open(client_log_path);

    Client_socket client("127.0.0.1", 8080);
    
    string msg = "hello Server";
    MsgHead msg_head = {1, static_cast<uint16_t>(msg.size())};

    char data[30] = {0};
    memcpy(data, &msg_head, sizeof(msg_head));
    memcpy(data + sizeof(msg_head), msg.c_str(), msg.size());

    client.send(data, sizeof(data));

    char buffer[1024] = {0};
    client.recv(buffer, sizeof(buffer));
    printf("recv: %s\n", buffer); 

    client.close();
    return 0;
}