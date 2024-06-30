#pragma once
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "Logger.h"

using namespace breeze::utility;
using std::string;

namespace breeze::socket
{

    class Socket
    {
    public:
        Socket();
        Socket(int sockfd);
        virtual ~Socket() = default;  

        int fd()const;
        bool bind(const string& ip, int port);
        bool listen(int backlog);
        bool connect(const string& ip, int port);
        int accept();
        int send(const char* buf, int len);
        int recv(char* buf, int len);
        void close();

        bool set_non_blocking();
        bool set_send_buffer(int size);
        bool set_recv_buffer(int size);
        bool set_linger(bool active, int seconds);
        bool set_keepalive();
        bool set_reuse_addr();
    protected:
        string m_ip;
        int m_port;
        int m_sockfd;
    };

    class Client_socket : public Socket
    {
    public:
        Client_socket() = delete;
        Client_socket(const string& ip, int port);
        ~Client_socket() = default;
    };

    class Serversocket : public Socket
    {
    public:
        Serversocket() = delete;
        Serversocket(const string& ip, int port);
        ~Serversocket() = default;
    };
}