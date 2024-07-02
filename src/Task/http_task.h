#pragma once
#include "task.h"
using namespace breeze::thread;

namespace breeze
{
    namespace task
    {
        class HttpTask : public Task
        {
        public:
            HttpTask() = delete;
            HttpTask(int sockfd);
            ~HttpTask();

            virtual void run();
            virtual void destroy();
        private:
            int m_sockfd = 0;
            bool m_closed = false;
        };
    }
}