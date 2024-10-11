/***************************************************************************
 * This file contains a simple and lightweight logger class declaration 
 * and useful logger macros for different log levels.
 * 
 * The logger is supposed to be a singleton object that provides a function 
 * to push messages to the write queue. Messages are processed in a separate 
 * write loop which runs in a dedicated thread.
 * 
 * Atm, the logger can be configured to:
 * - write messages to file only 
 * - write messages both to file and stdout
 * 
 * TODO: 
 * - add support for any stream
 * - break down to smalled classes
 * - log destination can be configurable
 * 
 *
 *  support: fedora0xffff
 ****************************************************************************/

#pragma once
#include "Config/LoggerConfig.h"
#include <pthread.h>
#include <thread>
#include <chrono>
#include <sstream>
#include <string>
#include <mutex>
#include <filesystem>
#include <queue>

 
#ifdef DISABLE_LOG
#define LOG_INFO(...) {}
#define TRACE {}
#define LOG_ERROR(...) {}
#define LOG_DBG(...) {}
#else

#define LOG_INFO(...) { \                                                                          
    std::ostringstream os__;                                                                        \
    os__ << "[INFO]: " << __LINE__ <<  ": " << __FUNCTION__ <<  ": ";                               \
    logger::Logger::getInstance().log(os__.str(),__VA_ARGS__);                             \
}

#define LOG_ERROR(...) {             \
    std::ostringstream os__;                                                                        \
    os__  << "[ERROR]: " << __LINE__ << ": " << __FUNCTION__ << ": ";                               \
    logger::Logger::getInstance().log(os__.str(),__VA_ARGS__);                             \
}
#ifndef NDEBUG
#define LOG_DBG(...) { \                                                                          
    std::ostringstream os__;                                                                        \
    os__ << "[DEBUG]: " << __LINE__ << ": " << __FUNCTION__ << ": ";                                \
    logger::Logger::getInstance().log(os__.str(),__VA_ARGS__);                             \
}
#define TRACE {             \
   std::ostringstream os__;                                                                         \
    os__ << "[TRACE]: " << __LINE__ << ": " << __FUNCTION__ ;                                       \
    logger::Logger::getInstance().log(os__.str());                                         \
}
#else
#define LOG_DBG(...) {}
#define TRACE {}
#endif //NDEBUG

#endif //DISABLE_LOG
namespace logger 
{
    class Logger {
        Logger();
        Logger(const Logger&) = delete;

        void writeLog();
        std::string getLoggerFile();
    private:
        std::string getFileTimeStamp() const;
        std::string getEntryTimeStamp() const;
        void startWriteLoop();
    public:

        static Logger& getInstance();

        void setWriteToStdout(bool writeToStdout);
        void setPrintLogStartStop(bool printStartStop);
        void setLogFilePath(const std::string&);

        template<class... Ts>
        void log(Ts&&... args) {
            buf.clear();
            buf.str("");

            std::lock_guard lock(queue_mtx);
            (buf << ... << std::forward<Ts>(args));
            message_queue.push(buf.str());
        }

        virtual ~Logger();

    private:
        bool logger_thread_running;
        std::string log_file;
        std::mutex queue_mtx;
        std::thread logger_thread;
        std::stringstream buf;
        std::queue<std::string> message_queue;
        LoggerConfig config;
    };
} //namespace logger
