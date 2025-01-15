/***************************************************************************
 * This file contains json util functions and specializations based on
 * nlohmann::json
 *
 *  support: fedora0xffff
 ****************************************************************************/

#pragma once
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>
#include <vector>

#include "json.hpp"

namespace json_utils {
void saveJson(const std::string& path, const nlohmann::json& object);
nlohmann::json loadJson(const std::string& path);

template <typename T>
T getValue(const nlohmann::json& config, const std::string& key,
		   const T& defaultValue) {
	T result = defaultValue;
	if (config.find(key) != config.end()) {
		if (!config.at(key).is_null()) {
			try {
				result = config.at(key).get<T>();
			} catch (const nlohmann::json::exception& e) {
				std::cerr << e.what() << std::endl;
			}
		} else {
			std::cerr << "Value " << key << " found as null" << std::endl;
		}
	} else {
		std::cerr << "Value " << key << " not found" << std::endl;
	}
	return result;
}

template <typename T>
T getStruct(const nlohmann::json& config, const std::string& key,
			const T& defaultValue) {
	if (config.find(key) != config.end()) {
		return config.at(key);
	}

	std::cerr << "Struct not found: " << key << std::endl;

	return defaultValue;
}

template <typename T>
T getStruct(const nlohmann::json& config, const std::string& key) {
	if (config.find(key) != config.end()) {
		return config.at(key);
	}
	throw std::runtime_error("JSON file does not conatin the required field: " +
							 key);
}

template <typename T>
struct JsonUtils {
	static std::vector<T> getVector(const nlohmann::json& config,
									const std::string& key) {
		std::vector<T> values;
		if (config.find(key) != config.end()) {
			std::for_each(
				std::begin(config.at(key)), std::end(config.at(key)),
				[&values](auto&& item) { values.emplace_back(T(item)); });
		}
		return values;
	}

	static std::list<T> getList(const nlohmann::json& config,
								const std::string& key) {
		std::list<T> values;
		if (config.find(key) != config.end()) {
			std::for_each(
				std::begin(config.at(key)), std::end(config.at(key)),
				[&values](auto&& item) { values.emplace_back(T(item)); });
		}
		return values;
	}

	static std::map<std::string, T> getMap(const nlohmann::json& config,
										   const std::string& key) {
		std::map<std::string, T> values;
		if (config.find(key) != config.end()) {
			auto semiMap =
				config.at(key).get<std::map<std::string, nlohmann::json>>();
			std::for_each(std::begin(semiMap), std::end(semiMap),
						  [&values](const auto& item) {
							  values[item.first] = T(item.second);
						  });
		}
		return values;
	}
};

template <>
struct JsonUtils<std::string> {
	static std::vector<std::string> getVector(const nlohmann::json& config,
											  const std::string& key) {
		std::vector<std::string> values;
		if (config.find(key) != config.end()) {
			std::for_each(
				std::begin(config.at(key)), std::end(config.at(key)),
				[&values](const auto& item) {
					values.emplace_back(item.template get<std::string>());
				});
		}
		return values;
	}
};
}  // namespace json_utils
