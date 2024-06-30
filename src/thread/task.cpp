#include "task.h"
using namespace breeze::thread;

Task::Task() : m_data(nullptr)
{
}

Task::Task(void * data) : m_data(data)
{
}

Task::~Task()
{
}

void * Task::get_data()
{
    return m_data;
}

void Task::set_data(void * data)
{
    m_data = data;
}
