#include "task_factory.h"
#include "Logger.h"
#include "http_task.h"
#include <mutex>
#include <unistd.h>

using namespace breeze::task;

std::shared_ptr<Task> TaskFactory::create(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    if (it != m_sock_task.end())
    {
        log_debug("task_factory return a existing task: %x", it -> second.get());
        return it -> second;
    }
    std::shared_ptr<Task> task = std::make_shared<HttpTask>(sockfd);

    m_sock_task[sockfd] = task;

    log_debug("task_factory return a new task: %x", task.get());
    return task;
}

void TaskFactory::remove(int sockfd)
{
    std::unique_lock<std::mutex> lock(m_mutex);
    auto it = m_sock_task.find(sockfd);
    
    if (it != m_sock_task.end())
    {
        m_sock_task.erase(it);
    }

    close(sockfd);
}