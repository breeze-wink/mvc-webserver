#include "task_dispatcher.h"
#include "thread_pool.h"
#include "Logger.h"
#include <chrono>
#include <thread>
using namespace breeze::thread;

void TaskDispatcher::init(int threads)
{
    Singleton<ThreadPool>::Instance()->create(threads);
    start();
}

void TaskDispatcher::assign(std::shared_ptr<Task>&& task)
{
    log_debug("task dispatcher receive and ready to assign task: %x", task.get());
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        log_debug("task : %x, use_count = %d", task.get(), task.use_count());

        m_queue.push_back(std::move(task));

        log_debug("before notify, m_queue.size = %d", m_queue.size());
    }
    m_cond.notify_one();

    log_debug("task dispatcher push a task into queue");
}

void TaskDispatcher::handle(std::shared_ptr<Task>&& task)
{
    auto pool = Singleton<ThreadPool>::Instance();
    if (!pool->empty())
    {
        log_debug("task allocated to threadpool : %x", task.get());
        pool->assign(std::move(task));
        log_debug("back from pool assign");
    }
    else
    {
        std::unique_lock<std::mutex> lck(m_mutex);
        log_warn("all threads are busy!");
        m_queue.push_front(std::move(task));
    }
}

bool TaskDispatcher::empty()
{
    std::unique_lock<std::mutex> lck(m_mutex);
    return m_queue.empty();
}

void TaskDispatcher::stop()
{
    m_stop.store(true);
}

bool TaskDispatcher::is_stopped() const
{
    return m_stop.load();
}

void TaskDispatcher::run()
{
    log_debug("taskdispatcher thread id = %x", std::this_thread::get_id());
    while (!is_stopped())
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        while (m_queue.empty() && !is_stopped())
        {
            log_debug("task dispatcher enter wait");
            m_cond.wait_for(lck, std::chrono::seconds(1));
            log_debug("wake up from wait");
        }


        if (is_stopped() || m_queue.empty())
            break;

        auto task = std::move(m_queue.front());
        log_debug("TaskDispatcher take a task from queue: %x", task.get());

        m_queue.pop_front();
        lck.unlock();

        log_debug("ready to handle task: %x", task.get());
        handle(std::move(task));
    }
    log_debug("taskdispatcher over");
    
    Singleton<ThreadPool>::Instance()->stop();
}
