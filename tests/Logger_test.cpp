#define BOOST_TEST_MODULE LOGGER
#include "mini_logger.h"

#include <stdio.h>

#include <boost/test/unit_test.hpp>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

namespace fs = std::filesystem;

// TODO make test utils like a script class that inherits from file

namespace test_utils {

}  // namespace test_utils

BOOST_AUTO_TEST_CASE(logger_test1) {
	BOOST_CHECK(true);
}

BOOST_AUTO_TEST_CASE(logger_test2) {
	BOOST_CHECK(true);
}
