#include "Logger.h"
#include <iomanip>
#include <fstream>
#include <iostream>

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
log_file(getLoggerFile()),  
config(LoggerConfig::load()) {
    if (!logger_thread_running) {
        logger_thread_running = true;
        logger_thread = std::thread([&]() {
            this->startWriteLoop();
        });
    }
}

logger::Logger& logger::Logger::getInstance() {
    static Logger obj;
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

std::string logger::Logger::getLoggerFile() {
    auto path = config.logsAbsolutePath();
    std::ostringstream os;

    try {
        if (fs::is_directory(path) || fs::create_directory(path)) {
            os << path << '/' << LoggerConfig::logFileName() << getFileTimeStamp() << ".log";
            std::ofstream log_stream;
            log_stream.open(log_file, std::ios::trunc);
            log_stream.close();
        }
    }
    catch (std::exception& ex) {
        std::cout << "Failed to create the log file, restart as sudo" << std::endl;
    }

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
    config.writeToStdout_ = writeToStdout;
    config.save();
}

void logger::Logger::setPrintLogStartStop(bool printLogStartStop) {
     config.printStartStop_ = printLogStartStop;
     config.save();
}

void logger::Logger::setLogFilePath(const std::string& path) {
    config.setLogFilePath(path);
    config.save();
}

void logger::Logger::writeLog() {
    std::ofstream log_stream(log_file, std::ios::out | std::ios::app);
    
    while (!message_queue.empty()) {
        if (log_stream.is_open()) {
            log_stream << getEntryTimeStamp() << ": " << message_queue.front() << '\n';
        }
        if (config.writeToStdout_) {
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