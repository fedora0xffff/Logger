cmake_minimum_required(VERSION 3.10)
project(minilogger)  
set(target_demo "${PROJECT_NAME}_demo")
set(target_lib "${PROJECT_NAME}")

set(PROJECT_VERSION 1)
set(PROJECT_VERSION_MAJOR 0)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(PROJECT_DESCRIPTION "A c++ mini-logger library")

find_package(Threads REQUIRED)
message(STATUS "${Threads}")

set(OUTPUTCONFIG "${CMAKE_BUILD_TYPE}")
if ("${OUTPUTCONFIG}" STREQUAL "")
set(OUTPUTCONFIG debug)
endif()
string(TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG)
message(STATUS "${OUTPUTCONFIG}")

#demo target
add_executable(${target_demo}
  ${CMAKE_SOURCE_DIR}/sources/LoggerConfig.cpp  
  ${CMAKE_SOURCE_DIR}/sources/JsonUtils.cpp  
  ${CMAKE_SOURCE_DIR}/sources/Logger.cpp 
  ${CMAKE_SOURCE_DIR}/demo/main.cpp 
) 

target_include_directories(${target_demo} PUBLIC 
  ${CMAKE_SOURCE_DIR}/src/3rdparty/json
  ${CMAKE_SOURCE_DIR}/src 
)

target_link_libraries(${target_demo} 
  PUBLIC ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties(${target_demo} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}"
)

# library target
add_library(${target_lib} SHARED
  sources/LoggerConfig.cpp
  sources/JsonUtils.cpp
  sources/Logger.cpp
  )

set(API "headers/mini_logger.h;headers/JsonUtils.h;headers/LoggerConfig.h;headers/3rdparty/json/json.hpp")
#set(API "headers/mini_logger.h")

include(GNUInstallDirs)

set_target_properties(
  ${target_lib} 
  PROPERTIES 
  VERSION "${PROJECT_VERSION}"
  SOVERSION "${PROJECT_VERSION_MAJOR}"
  PUBLIC_HEADER "${API}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib_${OUTPUTCONFIG}"
)

set(CONF_FILE "${target_lib}.pc.in")
configure_file(${CONF_FILE} "${CMAKE_SOURCE_DIR}/${target_lib}.pc" @ONLY)
target_include_directories(${target_lib} PUBLIC 
  ${CMAKE_SOURCE_DIR}/headers
  ${CMAKE_SOURCE_DIR}/headers/3rdparty/json
)

install(TARGETS ${target_lib}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(FILES "${CMAKE_SOURCE_DIR}/${target_lib}.pc" 
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig"
)

# boost tests section

find_library(boost-test 
    NAMES libboost_unit_test_framework.so
)

if(boost-test)
    message(STATUS "Boost test library found")
    ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)

    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    include(boost-tests-helper)

    # TODO: how to invoke tests automatically on a debug build type?
    # можно конечно и без этого, а просто в скрипте билда вызвать cmake --build --target test
    add_boost_test(tests/Logger_test.cpp ${target_lib})
    add_boost_test(tests/General_test.cpp ${target_lib})

    # запускать можно либо так: ctest .. 
    # либо так: cmake --build . --target test   
    # либо так: make test
endif()