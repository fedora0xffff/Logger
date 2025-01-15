#include "LoggerConfig.h"

#include <stdlib.h>

#include <exception>
#include <iostream>
#include <sstream>

using namespace json_utils;
using json = nlohmann::json;

#define CERR(text) \
	(std::cerr << __FILE__ << ' ' << __LINE__ << ": " << text << std::endl)

namespace {
using namespace std::literals;
const std::string log_default_path = "/var/log/logger_directory";
const std::string log_default_file_name = "mini-logger";
// TODO: atm, support only a user config
const auto user_config_name = "logConfig.json"sv;
const auto config_path = "/etc/logger"sv;

std::string getEnv(const std::string& str) {
	if (auto res = getenv(str.c_str())) {
		return res;
	}
	return {};
}
}  // namespace

logger::LoggerConfig::LoggerConfig(const json& config)
	: log_file_name(getValue(config, "log_file_name", log_default_file_name)),
	  log_file_path(getValue(config, "log_file_path", log_default_path)),
	  print_start_stop(getValue(config, "print_start_stop", false)),
	  disable_logger(getValue(config, "disable_logger", false)),
	  log_to_stdout(getValue(config, "log_to_stdout", false)) {}

logger::LoggerConfig::operator nlohmann::json() const {
	return {
		{"print_start_stop", print_start_stop},
		{"disable_logger", disable_logger},
		{"log_to_stdout", log_to_stdout},
		{"log_file_path", log_file_path},
		{"log_file_name", log_file_name},
	};
}

logger::LoggerConfig logger::LoggerConfig::load(const std::string& path) {
	try {
		json obj = loadJson(path);
		return LoggerConfig(obj);
	} catch (const std::exception& e) {
		CERR(e.what());
	}
	return LoggerConfig();
}

std::string logger::LoggerConfig::configAbsolutePath() {
	return config_path.data();
}

std::string logger::LoggerConfig::logsAbsolutePath() const {
	const auto home = getEnv("HOME");
	return home + "/" + log_file_path;
}

std::string logger::LoggerConfig::getLogFileName() { return log_file_name; }

void logger::LoggerConfig::save(const std::string& path) const {
	try {
		saveJson(path, (json) * this);
	} catch (const std::exception& e) {
		CERR(e.what());
	}
}

void logger::LoggerConfig::setLogFilePath(const std::string& path) {
	log_file_path = path;
}

void logger::LoggerConfig::setLogFileName(const std::string& name) {
	log_file_name = name;
}
void logger::LoggerConfig::setPrintLogStartStop(bool print_start) {
	print_start_stop = print_start;
}

void logger::LoggerConfig::setLogToStdout(bool log_stdout) {
	log_to_stdout = log_stdout;
}

void logger::LoggerConfig::setDisableLogger(bool disable) {
	disable_logger = disable;
}

bool logger::LoggerConfig::getPrintLogStartStop() const {
	return print_start_stop;
}

bool logger::LoggerConfig::getLogToStdout() const { return log_to_stdout; }

bool logger::LoggerConfig::getDisableLogger() const { return disable_logger; }
