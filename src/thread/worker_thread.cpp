#include "worker_thread.h"
#include "Logger.h"
#include "task_dispatcher.h"
#include "thread_pool.h"
#include <chrono>


using namespace breeze::utility;
using namespace breeze::thread;

WorkerThread::WorkerThread() : Thread(), m_task(nullptr)
{
    log_debug("WorkerThread created: %x", this);
}

WorkerThread::~WorkerThread()
{
    log_debug("WorkerThread destroyed: %x", this);
}

void WorkerThread::run()
{
    while (!Singleton<TaskDispatcher>::Instance()->is_stopped())
    {
        std::unique_lock<std::mutex> lck(m_mutex);

        while (m_task == nullptr && !Singleton<TaskDispatcher>::Instance()->is_stopped())
        {
            m_cond.wait_for(lck, std::chrono::seconds(1));
        }
            

        if (Singleton<TaskDispatcher>::Instance()->is_stopped() || m_task == nullptr)
            break;

        lck.unlock();

        log_debug("worker thread run: thread=%x, task=%x", this, m_task.get());

        m_task -> run();
        m_task -> destroy();
        m_task = nullptr;

        Singleton<ThreadPool>::Instance()->put(this);
    }
}

void WorkerThread::assign(std::shared_ptr<Task> task)
{
    std::unique_lock<std::mutex> lck(m_mutex);
    m_task = task;
    m_cond.notify_one();
}
