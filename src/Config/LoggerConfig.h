#include "../Json/JsonUtils.h"

namespace logger {

    /// @brief logger info struct. All info is stored as json: 
    /// write messages for logger start and stop,
    /// write all messages to stdout (writes to file too)
    /// if logger should be disabled
    struct LoggerConfig final
    {
        LoggerConfig() = default;
        explicit LoggerConfig(const nlohmann::json& config);
        operator nlohmann::json() const;
        void save(const std::string& path = configAbsolutePath()) const;
    public:
        static LoggerConfig load(const std::string& path = configAbsolutePath());
        static std::string configAbsolutePath();
        static std::string logsAbsolutePath();
        static std::string logFileName();
    public:
        bool printStartStop_ = false;
        bool writeToStdout_ = false;
        bool disableLogger_ = false;
    };
} //namespace logger
