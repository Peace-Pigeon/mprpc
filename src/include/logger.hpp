#pragma once
#include "lockqueue.hpp"

// Mprpc框架日志系统
enum LogLevel{
    INFO,       // 普通信息
    ERROR,      // 错误信息
};

class Logger
{
public:
    // 获取日志的单例
    static Logger& GetInstance();
    // 设置日志的级别
    void SetLogLevel(LogLevel level);
    // 写日志
    void Log(std::string msg);

private:
    int m_loglevel;                     // 记录日志级别
    LockQueue<std::string> m_lockQue;   // 日志\缓冲队列
    Logger();
    Logger(const Logger&)=delete;
    Logger(Logger&&)=delete;
};

// 定义宏,格式是 LOG_INFO("XXX %d %s",20,"xxx")
#define LOG_INFO(logmsgformat,...) \
    do{ \
        Logger &logger=Logger::GetInstance(); \
        logger.SetLogLevel(INFO); \
        char logmsg[1024]={0}; \
        snprintf(logmsg,1024,logmsgformat,##__VA_ARGS__); \
        logger.Log(logmsg); \
    }while(0);

#define LOG_ERROR(logmsgformat,...) \
    do{ \
        Logger &logger=Logger::GetInstance(); \
        logger.SetLogLevel(ERROR); \
        char logmsg[1024]={0}; \
        snprintf(logmsg,1024,logmsgformat,##__VA_ARGS__); \
        logger.Log(logmsg); \
    }while(0);
