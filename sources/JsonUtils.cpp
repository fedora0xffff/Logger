#include "../headers/JsonUtils.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
using json = nlohmann::json;
void json_utils::saveJson(const std::string& path, const json& object) {
	// TODO: try save to /tmp and then move to path
	const auto dump = object.dump(4);
	std::ofstream out{path};
	out << dump;
}

json json_utils::loadJson(const std::string& path) {
	std::ifstream in{path};

	if (!in.is_open()) {
		throw std::runtime_error("Failed to open file: " + path);
	}

	std::stringstream ss;
	ss << in.rdbuf();
	const json result = json::parse(ss.str());
	return result;
}
