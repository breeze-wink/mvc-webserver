#include "work_task.h"
#include "Logger.h"
#include "socket.h"
#include "workflow.h"
#include "socket_handler.h"

using namespace breeze::task;
using namespace breeze::utility;
using namespace breeze::socket;
using namespace breeze::engine;

WorkTask::WorkTask(int sockfd) : Task(), m_sockfd(sockfd)
{
}

void WorkTask::run()
{
    log_debug("work task run");
    //接收客户端数据
    MsgHead head;
    memset(&head, 0, sizeof(head));

    Socket socket(m_sockfd);

    int len = socket.recv((char*) &head, sizeof(head));
    
    if (len < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK) //阻塞模式下，客户端异常退出处理, 非阻塞下，recv读空，send写满缓冲区会触发
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
        return;
    }
   
    if (len != sizeof(head))
    {
        log_error("msg head length incorrect: %d", len);
        m_closed = true;
        return;
    }

    log_debug("msg head: cmd = %d, len = %d", head.cmd, head.len);

    if (head.len > recv_buf_size)
    {
        log_error("msg body_len too large: len = %d", head.len);
        m_closed = true;
        return;
    }

    char buf[recv_buf_size] = {0};

    len = socket.recv(buf, head.len);

    if (len != head.len)
    {
        log_error("msg recv error");
        m_closed = true;
        return;
    }

    log_debug("msg body: len = %d, data = %s", len, buf);

    auto workflow = Singleton<Workflow>::Instance();

    string output;

    workflow -> run(static_cast<int>(head.cmd), buf, output);

    //发送回去
    socket.send(output.c_str(), output.size());


}

void WorkTask::destroy()
{
    log_debug("work task destroy");
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