#include "thread.h"
using namespace breeze::thread;

Thread::Thread()
{
}

Thread::~Thread()
{
}

void Thread::start()
{
    m_thread = std::thread(Thread::thread_func, this);
}

void Thread::join()
{
    m_thread.join();
}

void * Thread::thread_func(void * ptr)
{
    auto thread = (Thread *)ptr;
    thread->run();
    return ptr;
}
