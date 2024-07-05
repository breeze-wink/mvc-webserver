#include "task_factory.h"
#include "http_task.h"
#include <mutex>
#include <unistd.h>

using namespace breeze::task;

Task* TaskFactory::create(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    if (it != m_sock_task.end())
    {
        return it -> second;
    }
    auto task = new HttpTask(sockfd);
    m_sock_task[sockfd] = task;

    return task;
}

void TaskFactory::remove(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    
    if (it != m_sock_task.end())
    {
        delete it -> second;
        m_sock_task.erase(it);
    }

    close(sockfd);
}