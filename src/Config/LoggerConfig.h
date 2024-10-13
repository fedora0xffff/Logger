#include "../Json/JsonUtils.h"

namespace logger {

    /// @brief logger info class. All info is stored as json: 
    /// write messages for logger start and stop,
    /// write all messages to stdout (writes to file too)
    /// if logger should be disabled
    class LoggerConfig final
    {
    public:
        LoggerConfig() = default;
        explicit LoggerConfig(const nlohmann::json& config);
        operator nlohmann::json() const;
        void save(const std::string& path = configAbsolutePath()) const;
        void setLogFilePath(const std::string& path);
    public:
        std::string logsAbsolutePath() const;
        static LoggerConfig load(const std::string& path = configAbsolutePath());
        static std::string configAbsolutePath();
        static std::string logFileName();
    // TODO: add getters,setters for the fields below
        bool print_start_stop = false;
        bool log_to_stdout = false;
        bool disable_logger = false;
    private:
        std::string log_file_path;
    };
} //namespace logger
