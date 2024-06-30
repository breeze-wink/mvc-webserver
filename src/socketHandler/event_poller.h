#pragma once
#include <sys/epoll.h>
#include <sys/types.h>
#include <unistd.h>
namespace breeze
{
    namespace socket
    {
        class EventPoller
        {
        public:
            EventPoller();
            ~EventPoller();

            bool create(int max_conn);
            void add(int fd, u_int32_t events);
            void mod(int fd, u_int32_t events);
            void del(int fd);

            int wait(int timeout); // -1 无限等待, 0立即返回， >0 等待x milliseconds
            bool is_set(int idx, u_int32_t events);
            int get_fd(int idx);
        private:
            int m_epfd; //epoll 专用的文件描述符
            int m_max_conn; // epoll 的最大连接数
            struct epoll_event* m_events;  //epoll 用于回传待处理事件的数组
        };
    }
}   