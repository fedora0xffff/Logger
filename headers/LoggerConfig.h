
/***************************************************************************
 * logger info class. All info is stored as json:
 * - write messages for logger start and stop,
 * - write all messages to stdout (writes to file too)
 * - if logger should be disabled,
 * - logs directory path
 *
 *
 *  support: fedora0xffff
 ****************************************************************************/

#include "JsonUtils.h"

namespace logger {

class LoggerConfig final {
   public:
	LoggerConfig() = default;
	explicit LoggerConfig(const nlohmann::json& config);
	operator nlohmann::json() const;
	void save(const std::string& path = configAbsolutePath()) const;

	void setLogFilePath(const std::string&);
	void setLogFileName(const std::string&);

	void setPrintLogStartStop(bool);
	void setLogToStdout(bool);
	void setDisableLogger(bool);

   public:
	bool getPrintLogStartStop() const;
	bool getLogToStdout() const;
	bool getDisableLogger() const;
	std::string getLogFileName();
	std::string logsAbsolutePath() const;

	static LoggerConfig load(const std::string& path = configAbsolutePath());
	static std::string configAbsolutePath();

   private:
	bool print_start_stop = false;
	bool log_to_stdout = false;
	bool disable_logger = false;
	std::string log_file_path;
	std::string log_file_name;
};
}  // namespace logger
