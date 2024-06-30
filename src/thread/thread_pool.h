#pragma once
#include <list>
#include "worker_thread.h"
#include "Singleton.h"

using namespace breeze::utility;

namespace breeze
{
    namespace thread
    {
        class ThreadPool
        {
            SINGLETON(ThreadPool);
        public:
            void create(int threads);
            void assign(Task * task);

            WorkerThread * get();
            void put(WorkerThread * thread);
            bool empty();

        private:
            int m_threads = 0;
            std::list<WorkerThread *> m_pool;
            std::mutex m_mutex;
            std::condition_variable m_cond;
        };

}}
