#include "socket_handler.h"
#include "Logger.h"
#include "Singleton.h"
#include "socket.h"
#include "task_dispatcher.h"
#include <unistd.h>
#include "task_factory.h"


using namespace breeze::thread;
using namespace breeze::socket;
using namespace breeze::task;

void SocketHandler::listen(const string& ip, int port)
{
    m_server = std::make_unique<Serversocket>(ip, port);
}
void SocketHandler::attach(int sockfd)
{
    m_epoll.add(sockfd, EPOLLIN|EPOLLHUP|EPOLLERR|EPOLLONESHOT);
}
void SocketHandler::detach(int sockfd)
{
    m_epoll.del(sockfd);
}

bool SocketHandler::is_stopped() const
{
    return m_stop;
}

void SocketHandler::stop()
{
    m_stop.store(true);
    m_server -> close();
}

void SocketHandler::handle(int max_conn, int timeout)
{
    m_epoll.create(max_conn);
    u_int32_t events = EPOLLIN|EPOLLHUP|EPOLLERR;
    m_epoll.add(m_server -> fd(), events);

    while (! is_stopped()) 
    {
        int num = m_epoll.wait(timeout); 
        if (num < 0) 
        {
            if (errno == EINTR) 
            {
                // epoll_wait 被信号中断，继续重试
                continue;
            } 
            else 
            {
                log_error("epoll wait error: errno = %d, errmsg = %s", errno, strerror(errno));
                break;
            }
        }
        else if (num == 0)
        {
            continue;
        }
        for (int i = 0; i < num; ++ i)
        {
            if (m_epoll.get_fd(i) == m_server -> fd()) // accepter
            {
                //服务端套接字可读
                int connfd = m_server -> accept();

                //将连接套接字添加到监听队列
                struct epoll_event ev2;
                Socket client(connfd);
                client.set_non_blocking();
                attach(connfd);
            }
            else
            {
                int connfd = m_epoll.get_fd(i);
                if (m_epoll.is_set(i, EPOLLHUP)) // 客户端异常退出会被epoll检测为EPOLLHUP
                {   
                    //连接套接字挂断了
                    log_error("socket hang up by peer: errno = %d, errmsg = %s", errno, strerror(errno));
                    detach(connfd);
                    ::close(connfd);               
                }
                else if (m_epoll.is_set(i, EPOLLERR))
                {   
                    //连接套接字出现错误  
                    log_error("socket error: connfd = %d, errno = %d, errmsg = %s", connfd, errno, strerror(errno));
                    detach(connfd);
                    ::close(connfd);               

                }
                else if (m_epoll.is_set(i, EPOLLIN))
                {
                    detach(connfd);

                    auto task = Singleton<TaskFactory>::Instance() -> create(connfd);

                    Singleton<TaskDispatcher>::Instance() -> assign(std::move(task));
                }

                
            }
        }
    }
}