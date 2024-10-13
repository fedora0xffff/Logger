#include "LoggerConfig.h"
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <sstream>

using namespace json_utils;
using json = nlohmann::json;

namespace
{
    using namespace std::literals;
    const std::string log_default_path = "/var/log/logger_directory";
    const auto config_name = "logConfig.json"sv;
    const auto config_path = "/etc/logger"sv;
    const auto log_file = "log"sv;

    std::string getEnv(const std::string& str) {
        if (auto res = getenv(str.c_str())) {
            return res;
        }
        return {};
    }
}

void printToCerr(std::string_view sv)
{
    std::cerr << __FILE__ << ' ' << __LINE__ << ": " << sv << std::endl;
}

logger::LoggerConfig::LoggerConfig(const json& config) 
: print_start_stop(getValue(config, "print_start_stop", false)),
log_file_path(getValue(config, "log_file_path", log_default_path)),
disable_logger(getValue(config, "disable_logger", false)),
log_to_stdout(getValue(config, "log_to_stdout", false))
{}

logger::LoggerConfig::operator nlohmann::json() const
{
    return 
    {
        { "print_start_stop", print_start_stop },
        { "disable_logger", disable_logger },
        { "log_to_stdout", log_to_stdout },
        { "log_file_path", log_file_path },
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
    { 
        printToCerr(e.what());
    }
    return LoggerConfig();
}

std::string logger::LoggerConfig::configAbsolutePath()
{
    return config_path.data();
}

std::string logger::LoggerConfig::logsAbsolutePath() const
{
    const auto home = getEnv("HOME");
    return home + "/" + log_file_path;
}

std::string logger::LoggerConfig::logFileName()
{
    return log_file.data();
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
