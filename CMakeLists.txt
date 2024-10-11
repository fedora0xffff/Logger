cmake_minimum_required(VERSION 3.10)
project(logger_demo)  

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/src/Config/LoggerConfig.cpp  
  ${CMAKE_SOURCE_DIR}/src/Json/JsonUtils.cpp  
  ${CMAKE_SOURCE_DIR}/src/Logger/Logger.cpp 
  ${CMAKE_SOURCE_DIR}/demo/main.cpp 
) 

find_package(Threads REQUIRED)
message(STATUS "${Threads}")

target_include_directories(${PROJECT_NAME} PUBLIC 
  ${CMAKE_SOURCE_DIR}/src/3rdparty/json
  ${CMAKE_SOURCE_DIR}/src 
)

target_link_libraries(${PROJECT_NAME} 
  PUBLIC ${CMAKE_THREAD_LIBS_INIT}
)

set(OUTPUTCONFIG "${CMAKE_BUILD_TYPE}")
if ("${OUTPUTCONFIG}" STREQUAL "")
  set(OUTPUTCONFIG debug)
endif()

string(TOUPPER "${OUTPUTCONFIG}" OUTPUTCONFIG)
message(STATUS "${OUTPUTCONFIG}")

set_target_properties(${PROJECT_NAME} 
    PROPERTIES
    #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}"
)
