#pragma once

#include <chrono>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>
#include <optional>
#include <mutex>
#include <thread>
#include <filesystem>
#include <syncstream>
#include <iostream>
#include <utility>
#include <queue>
#include <atomic>

using namespace std::literals;
namespace fs = std::filesystem;

#define LOG(...) Logger::GetInstance().Log(__VA_ARGS__)

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}

struct FileInfo
{
    int count;
    fs::path path;
};

class Logger {

    auto GetTime() const 
    {
        if (manual_ts_) 
        {
            return *manual_ts_;
        }
        return std::chrono::system_clock::now();
    }

    auto GetTimeStamp() const 
    {
        const auto now = GetTime();
        const auto t_c = std::chrono::system_clock::to_time_t(now);
        return std::put_time(std::localtime(&t_c), "%F %T");
    }

    Logger();
    Logger(const Logger&) = delete;

    void writeLog();
    void setLoggerFile();

    // Для имени файла возьмите дату с форматом "%Y_%m_%d"
    std::string GetFileTimeStamp() const;

public:

    static Logger& GetInstance() {
        static Logger obj;
        return obj;
    }

    // Выведите в поток все аргументы.
    template<class... Ts>
    void Log(Ts&&... args)
    {
        buf_.clear();
        buf_.str("");

        (buf_ << ... << std::forward<Ts>(args));

        std::lock_guard lock(mt_);
        messageQueue_.push(buf_.str());

        if (!logThreadRun_)
        {
            std::jthread{[&](){
                std::cout << "Started the log thread " << std::this_thread::get_id() << std::endl;
                this->writeLog();
            }}.detach();

            logThreadRun_ = true;
        }
    }

    ~Logger();

    // Установите manual_ts_. Учтите, что эта операция может выполняться
    // параллельно с выводом в поток, вам нужно предусмотреть 
    // синхронизацию.
    void SetTimestamp(std::chrono::system_clock::time_point ts);

private:
    std::optional<std::chrono::system_clock::time_point> manual_ts_;
    std::stringstream buf_;
    std::queue<std::string> messageQueue_;
    std::mutex mt_;
    bool logThreadRun_ = false;
    fs::path pathLogger_{"/home/anast/logger_test"};
    FileInfo logFile_;
    bool nothingToWrite_;
    bool terminate_;
    bool timeStampChanged_;
};


void  Logger::SetTimestamp(std::chrono::system_clock::time_point ts)
{
    manual_ts_ = ts;
    timeStampChanged_ = true;
}

Logger::Logger()
{
    setLoggerFile();
    auto currentFile = std::ofstream{logFile_.path, std::ofstream::app};
    std::osyncstream syncout{currentFile};
    syncout << "-----------Logger starts------------" << std::endl;
}

void Logger::setLoggerFile()
{
    std::string name = "sample_log_" + GetFileTimeStamp() + ".log";
    auto path = fs::path{pathLogger_/name};
    logFile_.path = path; 
}

// Для имени файла возьмите дату с форматом "%Y_%m_%d"
std::string Logger::GetFileTimeStamp() const
{
    const tm* now = GetTimeStamp()._M_tmb;
    std::string timeStampStr = std::to_string(now->tm_year + 1900) + '_' 
                        + std::to_string(now->tm_mon + 1) + '_' 
                        + std::to_string(now->tm_mday);
    return timeStampStr;
}

void Logger::writeLog()
{
    while (!messageQueue_.empty())
    {
        if (timeStampChanged_)
        {
            setLoggerFile();
        }
        
        auto currentFile = std::ofstream{logFile_.path, std::ofstream::app};
        std::osyncstream syncout{currentFile};
        std::cout << "log: " << GetTimeStamp() << ": " << messageQueue_.front() << '\n';
        syncout << GetTimeStamp() << ": " << messageQueue_.front() << std::endl;
        messageQueue_.pop();
    }
    logThreadRun_ = false;

    std::cout << "the log thread terminates " << std::this_thread::get_id() << std::endl;
}

Logger::~Logger()
{
    while (logThreadRun_) {}
    auto currentFile = std::ofstream{logFile_.path, std::ofstream::app};
    std::osyncstream syncout{currentFile};
    syncout << "-----------Logger stops------------" << '\n';
}
