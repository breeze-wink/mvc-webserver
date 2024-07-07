#pragma once

#include <memory>
#include <mutex>
#include <condition_variable>
#include "thread.h"
#include "task.h"

namespace breeze
{
    namespace thread
    {
        class WorkerThread : public Thread
        {
        public:
            WorkerThread();
            ~WorkerThread();

            virtual void run();
            void assign(std::shared_ptr<Task> task);

        private:
            std::shared_ptr<Task> m_task;
            std::mutex m_mutex;
            std::condition_variable m_cond;
        };
    }
}
