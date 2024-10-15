#include "../headers/mini_logger.h"
#include <iostream>
#include <iomanip>
#include <fstream>

namespace fs = std::filesystem;

namespace {
    template <typename TP>
    std::time_t to_time_t(TP tp) {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                + system_clock::now());
        return system_clock::to_time_t(sctp);
    }
}

logger::Logger::Logger() 
: logger_thread_running(false), 
config(LoggerConfig::load()) {
    createLoggerDir();
    if (!logger_thread_running && 
        config.getLogLevel() != logger::LogLevel::NONE) {
        logger_thread_running = true;
        logger_thread = std::thread([&]() {
            this->startWriteLoop();
        });
    }
}

logger::Logger& logger::Logger::getInstance() {
    static Logger obj{};
    return obj;
}

void logger::Logger::startWriteLoop() {
    using namespace std::literals;
    while(logger_thread_running) {
        if(!message_queue.empty()) {
            writeLog();
        }
    }
}

// TODO: add /var/log/mini-logger
void logger::Logger::createLoggerDir() const
{
    const auto log_path = config.logsAbsolutePath();
    if (!fs::is_directory(log_path)) {
        std::stringstream cmd;
        cmd << "mkdir -p " << log_path << " > /dev/null 2>&1";
        std::system(cmd.str().c_str());
    }
}

std::string logger::Logger::getLoggerFile()
{
    auto path = config.logsAbsolutePath();
    // log path might have been changed by the user
    if (!fs::is_directory(path)) {
        createLoggerDir();
    }
    std::ostringstream os;
    os << path << '/' << config.getLogFileName() << getFileTimeStamp() << ".log";
    return os.str();
}

std::string logger::Logger::getFileTimeStamp() const {
    const auto now = std::chrono::system_clock::now();
    const auto t_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream os;
    os << std::put_time(std::localtime(&t_c), "%F");
    return os.str();
}

std::string logger::Logger::getEntryTimeStamp() const {
    const auto now = std::chrono::system_clock::now();
    const auto t_c = std::chrono::system_clock::to_time_t(now);
    std::ostringstream os;
    os << std::put_time(std::localtime(&t_c), "%T");
    return os.str();
}


void logger::Logger::setWriteToStdout(bool writeToStdout) {
    config.setLogToStdout(writeToStdout);
    config.save();
}

void logger::Logger::setPrintLogStartStop(bool printLogStartStop) {
     config.setPrintLogStartStop(printLogStartStop);
     config.save();
}

void logger::Logger::setLogFilePath(const std::string& path) {
    config.setLogFilePath(path);
    config.save();
}

void logger::Logger::setLogFileName(const std::string& name)
{
    config.setLogFileName(name);
    config.save();
}

void logger::Logger::setLogLevel(logger::LogLevel level)
{
    config.setLogLevel(level);
    config.save();
}

void logger::Logger::writeLog()
{
    const auto log_file = getLoggerFile();
    std::ofstream log_stream(log_file, std::ios::out | std::ios::app);
    
    while (!message_queue.empty()) {
        if (log_stream.is_open()) {
            log_stream << getEntryTimeStamp() << ": " << message_queue.front() << '\n';
        }
        if (config.getLogToStdout()) {
            std::cout << getEntryTimeStamp() << ": " << message_queue.front() << std::endl;
        }
        message_queue.pop();
    }

    if (log_stream.is_open()) {
            log_stream << std::endl;
    }
}

logger::Logger::~Logger() {
    logger_thread_running = false;
    if (logger_thread.joinable()) {
        logger_thread.join();
    }
}