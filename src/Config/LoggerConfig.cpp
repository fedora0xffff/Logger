#include "LoggerConfig.h"
#include <sstream>
#include <iostream>
#include <exception>

using namespace json_utils;
using json = nlohmann::json;

namespace
{
    using namespace std::literals;
    const auto logConfigName = "logConfig.json"sv;
    const auto configPath = "~/.logger"sv;
    
    const auto logFile = "logger"sv;
}

void printToCerr(std::string_view sv)
{
    std::cerr << __FILE__ << ' ' << __LINE__ << ": " << sv << std::endl;
}

logger::LoggerConfig::LoggerConfig(const json& config) 
: printStartStop_(getValue(config, "printStartStop", false)),
writeToStdout_(getValue(config, "writeToStdout", false)),
disableLogger_(getValue(config, "disableLogger", false))
{}

logger::LoggerConfig::operator nlohmann::json() const
{
    return 
    {
        { "printStartStop", printStartStop_ },
        { "writeToStdout", writeToStdout_ },
        { "disableLogger", disableLogger_ },
    };
}

logger::LoggerConfig logger::LoggerConfig::load(const std::string& path)
{
    try
    {
        json obj = loadJson(path);
        return LoggerConfig(obj);
    }
    catch(const std::exception& e)
    { }
    return LoggerConfig();
}

std::string logger::LoggerConfig::configAbsolutePath()
{
    std::ostringstream os;
    os << configPath << '/' << logConfigName;
    return os.str();
}

std::string logger::LoggerConfig::logsAbsolutePath() const
{
    return log_file_path;
}

std::string logger::LoggerConfig::logFileName()
{
    return logFile.data();
}

void logger::LoggerConfig::save(const std::string &path) const
{
    try
    {
        saveJson(path, (json)*this);
    }
    catch(const std::exception& e)
    {
        printToCerr(e.what());
    }
}

void logger::LoggerConfig::setLogFilePath(const std::string& path)
{
    log_file_path = path;
}
