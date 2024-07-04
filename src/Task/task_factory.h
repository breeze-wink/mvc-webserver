#pragma once
#include "http_task.h"
#include "task.h"
// #include "echo_task.h"
// #include "work_task.h"

using namespace breeze::thread;

namespace breeze::task
{
    class TaskFactory
    {
    public:
    //TODO: 完善工厂逻辑
        static Task* create(int sockfd)
        {
            return new HttpTask(sockfd);
        }
    };
}