#include "Logger.h"
#include <cerrno>
#include <cstring>
#include <sstream>
#include <iostream>

using namespace breeze::utility;

const char* Logger::s_level[LOG_COUNT] =
{
    "DEBUG", "INFO", "WARN", "ERROR", "FATAL"
};


void Logger::open(const std::string &fileName)
{
    m_fileName = fileName;

    if (m_ofs.is_open())
    {
        m_ofs.close();
    }

    m_ofs.open(fileName, std::ios::app);
    if(m_ofs.fail())
    {
        std::cout << "errmsg = " << strerror(errno) << std::endl;
        throw std::runtime_error("open log file failed: " + fileName);
    }
    //这里考虑翻滚的时候要重置长度, 以下写法比重置为0更准确
    m_ofs.seekp(0, std::ios::end);
    m_len = (int)m_ofs.tellp();
}

void Logger::close()
{
    m_ofs.close();
}

void Logger::localtime(struct tm* time_info, const time_t* ticks)
{
#ifdef WIN32
    localtime_s(time_info, ticks);
#else
    localtime_r(ticks, time_info);
#endif
}

void Logger::sleep(int millseconds)
{
#ifdef WIN32
    Sleep(millseconds);
#else
    usleep(millseconds*1000);
#endif
}

void Logger::log(Level level, const char* file, int line, const char* format, ...)
{
    //忽略低级别的日志，保留高级别的日志
    if(m_level > level)
    {
        return;
    }
    if(m_ofs.fail())
    {
        return;
    }
    std::ostringstream oss;

    time_t ticks = time(nullptr); // 获取秒数
    tm time_info = {0}; //时间结构体，将各个属性初始化为0
    localtime(&time_info, &ticks); //将系统时间输入time_info
    //转化为特定格式存储
    char timestamp[32] = {0};
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", &time_info);

    int len = 0;
    const char* formater = "%s %s %s:%d ";

    //第一次计算长度，前两个参数为特殊用法
    len = snprintf(nullptr, 0, formater, timestamp, s_level[level], file, line);

    if(len > 0)
    {
        char* buffer = new char[len + 1]; //包括'\0'
        //实际写入
        snprintf(buffer, len + 1, formater, timestamp, s_level[level], file, line);
        buffer[len] = 0;//最后一位设为'\0'
        oss << buffer;
        m_len += len;
        delete[] buffer;
    }

    //以上内容处理，内容前的东西

    va_list arg_ptr; //处理可变参数...
    va_start(arg_ptr, format); //将arg_ptr 指向第一个可变参数
    len = vsnprintf(nullptr, 0, format, arg_ptr); //用法类似于snprintf
    va_end(arg_ptr); //清理va_list
    if(len > 0)
    {
        //如果长度大于零，则继续操作
        char* content = new char[len + 1];
        va_start(arg_ptr, format);
        vsnprintf(content, len + 1, format, arg_ptr);//实际将内容写入content
        va_end(arg_ptr);
        content[len] = 0;
        oss << content;
        m_len += len;
        delete[] content;
    }
    oss << "\n";

    const string& str = oss.str();

    m_ofs << str;
    m_ofs.flush();

    if(m_console)
    {
        std::cout << str;
    }

    if(m_maxSize > 0 && m_len >= m_maxSize)
    {
        //日志需要翻滚
        rotate();
    }
}

void Logger::setLevel(Level level)
{
    m_level = level;
}

void Logger::setMaxSize(int maxSize)
{
    m_maxSize = maxSize;
}

void Logger::rotate()
{
    close();
    sleep(1000);
    time_t ticks = time(nullptr);
    struct tm time_info = {0};
    localtime(&time_info, &ticks);
    char timestamp[32] = {0};
    strftime(timestamp, sizeof timestamp, ".%Y-%m-%d_%H-%M-%S", &time_info);

    string fileName = m_fileName + timestamp;
    //把写好的备份，再重新打开
    if(rename(m_fileName.c_str(), fileName.c_str()) != 0)
    {
        std::cout << m_fileName << " " << fileName << std::endl;
        throw std::logic_error("rename fileName failed");
    }
    open(m_fileName);
}

void Logger::setConsole(bool console)
{
    m_console = console;
}




