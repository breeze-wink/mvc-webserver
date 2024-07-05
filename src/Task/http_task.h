#pragma once
#include "task.h"
#include "request.h"
using namespace breeze::thread;
using namespace breeze::web;
namespace breeze
{
    namespace task
    {

        const uint32_t recv_buff_size = 1024 * 8;

        class HttpTask : public Task
        {
        public:
            HttpTask() = delete;
            HttpTask(int sockfd);
            ~HttpTask();

            void reset();

            virtual void run();
            virtual void destroy();

        private:
            int m_sockfd = 0;
            bool m_closed = false;
            Request m_req;
            int m_total_len;
            int m_head_len;
            int m_body_len;
            int m_body_idx;
            char* m_body;
        };
    }
}