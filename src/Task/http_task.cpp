#include "http_task.h"
#include "Logger.h"
#include "response.h"
#include "socket.h"
#include "server.h"
#include "socket_handler.h"
#include "request.h"
#include "task_factory.h"
#include <cstring>
#include <unistd.h>

using namespace breeze::socket;
using namespace breeze::task;
using namespace breeze::web;

HttpTask::HttpTask(int sockfd) : Task(), m_sockfd(sockfd)
{
    m_total_len = 0;
    m_head_len = 0;
    m_body_len = 0;
    m_body_idx = 0;
    m_body = nullptr;
}
HttpTask::~HttpTask()
{

}
void HttpTask::run()
{
    log_debug("http task run");

    char buf[recv_buff_size] = {0};
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
        return;
    }
    log_debug("recv: conn = %d, msg = \n%s", m_sockfd, buf);

    m_total_len += len;
    if (m_head_len == 0)
    {
        m_head_len = m_req.parse_header(buf, len);
        m_body_len = m_req.content_length();

        if (m_body_len > 0)
        {
            m_body = new char[m_body_len + 1];
            m_body[m_body_len] = '\0';
            memcpy(m_body, buf + m_head_len, len - m_head_len);
            m_body_idx = len - m_head_len;
        }
    }

    else //不是第一次，都是消息体里面的内容
    {
        memcpy(m_body + m_body_idx, buf, len);
        m_body_idx += len;
    }

    if (m_total_len - m_head_len >= m_body_len)
    {
        //消息接收完整
        if (m_body_len > 0)
        {
            m_req.parse_body(m_body, m_body_len);
        }
        string resp = Singleton<web::Server>::Instance() -> handle(m_req);
        socket.send(resp.c_str(), resp.size());

        reset();
    }
}

void HttpTask::destroy()
{
    log_debug("http task destroy");
    if (m_closed)
    {
        Singleton<TaskFactory>::Instance() -> remove(m_sockfd);
    }
    else
    {
        Singleton<SocketHandler>::Instance() -> attach(m_sockfd);
    }
}

void HttpTask::reset()
{
    m_total_len = 0;
    m_head_len = 0;
    m_body_len = 0;
    m_body_idx = 0;

    if (m_body != nullptr)
    {
        delete [] m_body;
        m_body = nullptr;
    }
    
}