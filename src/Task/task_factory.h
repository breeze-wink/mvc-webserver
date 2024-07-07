#pragma once
#include "task.h"
#include "Singleton.h"
#include <map>
#include <memory>
#include <mutex>
// #include "echo_task.h"
// #include "work_task.h"

using namespace breeze::thread;
using namespace breeze::utility;

namespace breeze::task
{
    class TaskFactory
    {
        SINGLETON(TaskFactory);
    public:
    //TODO: 完善工厂逻辑
        std::shared_ptr<Task> create(int sockfd);
        void  remove(int sockfd);

    private:
        std::map<int, std::shared_ptr<Task>> m_sock_task;
        std::mutex m_mutex;
    };
}