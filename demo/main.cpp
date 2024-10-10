#include "../src/Logger/Logger.h"
namespace fs = std::filesystem;

std::string getHelp(std::string dis_file) {
    std::stringstream help;
    help << "Usage: " << dis_file << "<log_file_path>\n"
        << "specify a path to save the log file to\n";
    return help.str();
}
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << getHelp(argv[0]) << std::endl;
        return 0;
    }

    std::string log_file = fs::weakly_canonical(argv[1]);


    return 0;
}