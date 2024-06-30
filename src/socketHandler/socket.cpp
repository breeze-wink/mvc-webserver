#include "socket.h"

using namespace breeze::socket;

Socket::Socket() : m_ip(""), m_port(0), m_sockfd(0)
{
    m_sockfd = ::socket(AF_INET, SOCK_STREAM, 0);
    if(m_sockfd == -1)
    {
        log_error("create socket error: errno = %d errmsg = %s", errno, strerror(errno));
    }
    log_debug("create socket success!");
}

Socket::Socket(int sockfd) : m_ip(""), m_port(0), m_sockfd(sockfd){}


int Socket::fd()const
{
    return m_sockfd;
}

bool Socket::bind(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    if(ip.empty())
    {
        sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else
    {
        inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr.s_addr);
    }
    sockaddr.sin_port = htons(port);

    if(::bind(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) == -1)
    {
        log_error("socket bind error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    log_debug("socket bind success: ip = %s, port = %d", ip.c_str(), port);
    return true;
}

bool Socket::listen(int backlog)
{
    if(::listen(m_sockfd, backlog) == -1)
    {
        log_error("socket listen errro, errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    log_debug("socket listening ...");
    return true;
}

bool Socket::connect(const string &ip, int port)
{
    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &sockaddr.sin_addr.s_addr);
    
    if(::connect(m_sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr)) < 0)
    {
        log_error("socket connect error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    m_ip = ip;
    m_port = port;
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd, nullptr, nullptr);
    if(connfd < 0)
    {
        log_error("socket accept error: errno = %d, errmsg = %s", errno, strerror(errno));
        return -1;
    }
    log_debug("socket accept success, connfd = %d", connfd);
    return connfd;
}

int Socket::send(const char *buf, int len)
{
    return ::send(m_sockfd, buf, len, 0);
}

int Socket::recv(char *buf, int len)
{
    return ::recv(m_sockfd, buf, len, 0);
}

void Socket::close()
{
    if(m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}

//一以下内容用来修改socket的设置

bool Socket::set_non_blocking()
{
    int flags = fcntl(m_sockfd, F_GETFL, 0);
    if (flags < 0)
    {
        log_error("socket set_non_blocking error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    flags |= O_NONBLOCK;
    if(fcntl(m_sockfd, F_SETFL, flags) < 0)
    {
        log_error("socket set_non_blocking error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }

    return true;
}

bool Socket::set_send_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_SNDBUF, &buff_size, sizeof(buff_size)) < 0)
    {
        log_error("socket set_send_buffer error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_recv_buffer(int size)
{
    int buff_size = size;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, &buff_size, sizeof(buff_size)))
    {
        log_error("socket set_send_buffer error: errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_linger(bool active, int seconds)
{
    struct linger l;
    memset(&l, 0, sizeof(l));
    l.l_onoff = active ? 1 : 0;
    l.l_linger = seconds;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_LINGER, &l, sizeof(l)) < 0)
    {
        log_error("socket set_linger error, errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_keepalive()
{
    int flag = 1; // true or false
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) < 0)
    {
        log_error("socket set_keepalive error, errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    return true;
}

bool Socket::set_reuse_addr()
{
    int flag = 1;
    if (setsockopt(m_sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) < 0)
    {
        log_error("socket set_keepalive error, errno = %d, errmsg = %s", errno, strerror(errno));
        return false;
    }
    return true;
}


Client_socket::Client_socket(const string& ip, int port) : Socket()
{
    connect(ip, port);
}

Serversocket::Serversocket(const string& ip, int port):Socket()
{   
    set_non_blocking();
    // set_recv_buffer(10 * 1024);
    // set_send_buffer(10 * 1024);
    // set_linger(true, 0);
    // set_keepalive();
    // set_reuse_addr();
    bind(ip, port);
    listen(1024);
}