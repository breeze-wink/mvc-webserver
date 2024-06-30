#include "echo_task.h"
#include "socket.h"
#include "socket_handler.h"
#include <unistd.h>
using namespace breeze::socket;
using namespace breeze::task;

EchoTask::EchoTask(int sockfd) : Task(), m_sockfd(sockfd)
{

}
EchoTask::~EchoTask()
{
}
void EchoTask::run()
{
    log_debug("echo task run");

    char buf[1024] = {0};
    Socket socket(m_sockfd);
    int len = socket.recv(buf, sizeof(buf));
    
    if (len < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) //阻塞模式下，客⼾端异常退出处理, 非阻塞下，recv读空，send写满缓冲区会触发
        {
            log_debug("socket recv/send would block: conn = %d", m_sockfd);
            return; //注意，这是正常的
        }
        
        //interrupt
        else if (errno == EINTR) //读写出现中断错误
        {   
            log_error("socket recv interrupted: conn = %d", m_sockfd);
            return;
        }

        log_error("socket connection abort: conn = %d", m_sockfd);
        m_closed = true;
        return;
    }

    if (len == 0)
    {
        log_debug("socket closed by peer : conn = %d", m_sockfd);
        m_closed = true;

    }
    else if (len > 0)
    {
        log_debug("recv: conn = %d, msg = %s", m_sockfd, buf);

        //发送回去
        socket.send(buf, sizeof(buf));
    }
}

void EchoTask::destroy()
{
    log_debug("echo task destroy");
    if (m_closed)
    {
        close(m_sockfd);
    }
    else
    {
        Singleton<SocketHandler>::Instance() -> attach(m_sockfd);
    }
    delete this;
}