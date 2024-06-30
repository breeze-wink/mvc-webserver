#include "event_poller.h"
#include <sys/epoll.h>


using namespace breeze::socket;

EventPoller::EventPoller() : m_epfd(0), m_max_conn(0), m_events(nullptr)
{

}

EventPoller::~EventPoller()
{
    if (m_epfd > 0)
    {
        close(m_epfd);
        m_epfd = 0;
    }
    if (m_events != nullptr)
    {
        delete[] m_events;
        m_events = nullptr;
    }
}

bool EventPoller::create(int max_conn)
{
    m_epfd = epoll_create(max_conn);
    if (m_epfd < 0)
    {
        return false;
    }
    m_events = new epoll_event[max_conn];
    m_max_conn = max_conn;
    return true;
}

void EventPoller::add(int fd, u_int32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(m_epfd, EPOLL_CTL_ADD, fd, &ev);
}

void EventPoller::mod(int fd, u_int32_t events)
{
    struct epoll_event ev;
    ev.events = events;
    ev.data.fd = fd;
    epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ev);
}

void EventPoller::del(int fd)
{
    epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, nullptr);
}

int EventPoller::wait(int timeout)
{
    return epoll_wait(m_epfd, m_events, m_max_conn, timeout);
}

bool EventPoller::is_set(int idx, u_int32_t events)
{
    return m_events[idx].events & events;
}

int EventPoller::get_fd(int idx)
{
    return m_events[idx].data.fd;
}