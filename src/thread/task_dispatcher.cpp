#include "task_dispatcher.h"
#include "thread_pool.h"
#include "Logger.h"
using namespace breeze::thread;

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::Instance()->create(threads);
    start();
}

void TaskDispatcher::assign(Task * task)
{
    log_debug("task dispatcher assign task: %x", task);
    std::unique_lock<std::mutex> lck(m_mutex);
    m_queue.push_back(task);
    m_cond.notify_one();
}

void TaskDispatcher::handle(Task* task)
{
    ThreadPool * pool = Singleton<ThreadPool>::Instance();
    if (!pool->empty())
    {
        pool->assign(task);
    }
    else
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        m_queue.push_front(task);
        log_warn("all threads are busy!");
    }
}

bool TaskDispatcher::empty()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_queue.empty();
}

void TaskDispatcher::run()
{
    while (true)
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        while (m_queue.empty())
        {
            m_cond.wait(lck);
        }
        Task * task = m_queue.front();
        m_queue.pop_front();
        lck.unlock();

        handle(task);
    }
}
