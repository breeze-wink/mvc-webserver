#pragma once

#include <thread>

namespace breeze
{
    namespace thread
    {
        class Thread //虚基类
        {
        public:
            Thread();
            virtual ~Thread();

            virtual void run() = 0;

            void start();
            void stop();

        protected:
            static void * thread_func(void * ptr);

        protected:
            std::thread m_thread;
        };
    }
}
