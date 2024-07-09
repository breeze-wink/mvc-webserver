#pragma once
#include "Singleton.h"
#include "event_poller.h"
#include "socket.h"
#include <atomic>
#include <memory>



using namespace breeze::utility;

namespace breeze
{
    namespace socket
    {
        class SocketHandler
        {
            SINGLETON(SocketHandler);

        public:
            void listen(const string& ip, int port);
            void attach(int sockfd);
            void detach(int sockfd);
            void handle(int max_conn, int timeout);
            bool is_stopped() const;
            void stop();

        private:
            std::unique_ptr<Socket> m_server = nullptr;
            EventPoller m_epoll;
            std::atomic<bool> m_stop;
        };
    }
}
