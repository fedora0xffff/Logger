#include "../src/Logger/Logger.h"
#include <chrono>
namespace fs = std::filesystem;

std::string getHelp(std::string dis_file) {
    std::stringstream help;
    help << "Usage: " << dis_file << "<log_file_path> <log_entries_number>\n"
        << "specify a path to save the log file to\n"
        << "specify a number of log entries. Default is 100\n"
        << "Example: " << dis_file << "~/demo_log 1000\n"
        << "as demo entries will have been created, total write time will be shown\n";
    return help.str();
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << getHelp(argv[0]) << std::endl;
        return 0;
    }

    std::string log_dir = fs::weakly_canonical(argv[1]);

    if (!fs::is_directory(log_dir)) {
        std::cout << "Directory does not exist!" << std::endl;
        return -1;
    }

    logger::Logger::getInstance().setLogFilePath(log_dir);
    int log_entries = 100;
    if (argc > 2) {
        log_entries = std::stoi(argv[2]);
    }
    // now, lets create many demo log entries
    auto begin = std::chrono::steady_clock::now();
    for (int i = 0; i < log_entries; i++) {
        LOG_INFO("Entry number ", i, "...");
    }
    auto end = std::chrono::steady_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
    std::cout << "Total write time: " << duration << "ms" << std::endl;
    return 0;
}