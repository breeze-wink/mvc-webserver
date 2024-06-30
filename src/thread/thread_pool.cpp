#include "thread_pool.h"
#include <Logger.h>
using namespace breeze::thread;

void ThreadPool::create(int threads)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_threads = threads;
    for (int i = 0; i < threads; i++)
    {
        auto thread = new WorkerThread();
        m_pool.push_back(thread);
        thread->start();
    }
    log_debug("thread pool create worker threads: %d", threads);
}

WorkerThread * ThreadPool::get()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    while (m_pool.empty())
        m_cond.wait(lck);
    auto thread = m_pool.front();
    m_pool.pop_front();
    return thread;
}

void ThreadPool::put(WorkerThread * thread)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_pool.push_back(thread);
    m_cond.notify_one();
}

bool ThreadPool::empty()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_pool.empty();
}

void ThreadPool::assign(Task *task)
{
    auto thread = get();
    thread->assign(task);
}
