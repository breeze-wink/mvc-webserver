#pragma once
#include "task.h"
#include <cstdint>
using namespace breeze::thread;

namespace breeze::task
{
    struct MsgHead
    {
        uint16_t cmd;
        uint16_t len;
    }; 

    const uint16_t recv_buf_size = 1024;

    class WorkTask : public Task
    {
    public:
        WorkTask() = delete;
        WorkTask(int sockfd);
        ~WorkTask() = default;

        virtual void run() override;
        virtual void destroy() override;

    private:
        int m_sockfd = 0;
        bool m_closed = false;
    };
}