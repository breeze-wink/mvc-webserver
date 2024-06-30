#pragma once
#include "Singleton.h"
#include <string>
#include <fstream>
#include <ctime>

#include <cstdarg>
#ifdef WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

using std::string;

namespace breeze
{
    namespace utility
    {
        #define log_debug(format, ...) \
            Singleton<Logger>::Instance() -> log(Logger::LOG_DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
        #define log_info(format, ...) \
            Singleton<Logger>::Instance() -> log(Logger::LOG_INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
        #define log_warn(format, ...) \
            Singleton<Logger>::Instance() -> log(Logger::LOG_WARN,  __FILE__, __LINE__, format, ##__VA_ARGS__)
        #define log_error(format, ...) \
            Singleton<Logger>::Instance() -> log(Logger::LOG_ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
        #define log_fatal(format, ...) \
            Singleton<Logger>::Instance() -> log(Logger::LOG_FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
        class Logger
        {
            SINGLETON(Logger);
        public :
            enum Level
            {
                LOG_DEBUG = 0,
                LOG_INFO,
                LOG_WARN,
                LOG_ERROR,
                LOG_FATAL,
                LOG_COUNT
            };



            void open(const string& fileName);
            void close();
            void log(Level level, const char* file, int line, const char* format, ...);//日志写入函数
            void setLevel(Level level);
            void setMaxSize(int maxSize);
            void setConsole(bool console);

        private:
            void rotate();
            void localtime(struct tm* time_info, const time_t* ticks);
            void sleep(int millseconds); //跨平台的localtime 和 sleep 函数
        private:
            string m_fileName;
            std::ofstream m_ofs;
            int m_maxSize = 0;
            int m_level = LOG_DEBUG;
            int m_len = 0;
            int m_console = true;
            static const char* s_level[LOG_COUNT];
        };
    }
}

