#pragma once
#include "task.h"
#include "echo_task.h"
#include "work_task.h"

using namespace breeze::thread;

namespace breeze::task
{
    class TaskFactory
    {
    public:
        static Task* create(int sockfd)
        {
            return new WorkTask(sockfd);
        }
    };
}