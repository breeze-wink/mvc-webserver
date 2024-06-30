#pragma once
#include "task.h"
using namespace breeze::thread;

namespace breeze
{
    namespace task
    {
        class EchoTask : public Task
        {
        public:
            EchoTask() = delete;
            EchoTask(int sockfd);
            ~EchoTask();

            virtual void run();
            virtual void destroy();
        private:
            int m_sockfd = 0;
            bool m_closed = false;
        };
    }
}