#pragma once

#include <list>
#include "thread.h"
#include "Singleton.h"
#include <memory>
#include <mutex>
#include <condition_variable>
#include "task.h"

using namespace breeze::utility;

namespace breeze
{
    namespace thread
    {
        class TaskDispatcher : public Thread
        {
            SINGLETON(TaskDispatcher);
        public:
            void init(int threads);
            void assign(std::shared_ptr<Task>&& task);
            void handle(std::shared_ptr<Task>&& task);
            bool empty();
            virtual void run();

        protected:
            std::list<std::shared_ptr<Task>> m_queue;
            std::mutex m_mutex;
            std::condition_variable m_cond;
        };
    }
}
