#include "../headers/LoggerConfig.h"
#include <stdlib.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <filesystem>
#include "LoggerConfig.h"

using namespace json_utils;
using json = nlohmann::json;
namespace fs = std::filesystem;

#define CERR(text) (std::cerr << __FILE__ << ' ' << __LINE__ << ": " << text << std::endl)

namespace
{
    using namespace std::literals;
    // TODO: only when runs as root 
    //const std::string log_default_path = "/var/log/mini-logger";
    const std::string log_default_user_path = ".mini-logger/logs";
    const std::string log_default_file_name = "mini-log-file";
    // atm, supporting only a user config
    const auto user_config_name = "logConfig.json"sv;
    const auto user_config_dir = ".mini-logger/config"sv;

    std::string getEnv(const std::string& str) {
        if (auto res = getenv(str.c_str())) {
            return res;
        }
        return {};
    }
}

logger::LoggerConfig::LoggerConfig(const json& config) 
: log_file_name(getValue(config, "log_file_name", log_default_file_name)),
log_file_path(getValue(config, "log_file_path", getEnv("HOME") + "/" + log_default_user_path)),
log_level(getValue(config, "log_level", LogLevel::INFO)),
print_start_stop(getValue(config, "print_start_stop", false)),
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
        { "log_file_name", log_file_name },
        { "log_level", static_cast<int>(log_level) },
    };
}

logger::LoggerConfig logger::LoggerConfig::load(const std::string& path)
{
    const auto dir = fs::path(path).remove_filename();
    if (!fs::is_directory(dir)) {
        std::stringstream cmd;
        cmd << "mkdir -p " << dir << " > /dev/null 2>&1";
        std::system(cmd.str().c_str());
    }

    try
    {
        json obj = loadJson(path);
        return LoggerConfig(obj);
    }
    catch(const std::exception& e)
    { 
        CERR(e.what());
    }
    return LoggerConfig();
}

std::string logger::LoggerConfig::configAbsolutePath()
{
    std::stringstream path;
    path << getEnv("HOME") << '/' << user_config_dir << '/' << user_config_name;
    return path.str();
}

std::string logger::LoggerConfig::logsAbsolutePath() const
{
    return log_file_path;
}

std::string logger::LoggerConfig::getLogFileName()
{
    return log_file_name;
}

void logger::LoggerConfig::save(const std::string &path) const
{
    try
    {
        saveJson(path, (json)*this);
    }
    catch(const std::exception& e)
    {
        CERR(e.what());
    }
}

void logger::LoggerConfig::setLogFilePath(const std::string& path)
{
    log_file_path = path;
}

void logger::LoggerConfig::setLogFileName(const std::string& name)
{
    log_file_name = name;
}
void logger::LoggerConfig::setPrintLogStartStop(bool print_start)
{
    print_start_stop = print_start;
}

void logger::LoggerConfig::setLogToStdout(bool log_stdout)
{
    log_to_stdout = log_stdout;
}

void logger::LoggerConfig::setDisableLogger(bool disable)
{
    disable_logger = disable;
}

void logger::LoggerConfig::setLogLevel(logger::LogLevel level)
{
    log_level = level;
}

logger::LogLevel logger::LoggerConfig::getLogLevel() const
{
    return log_level;
}

bool logger::LoggerConfig::getPrintLogStartStop() const
{
    return print_start_stop;
}

bool logger::LoggerConfig::getLogToStdout() const
{
    return log_to_stdout;
}

bool logger::LoggerConfig::getDisableLogger() const
{
    return disable_logger;
}
