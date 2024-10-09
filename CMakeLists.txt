cmake_minimum_required(VERSION 3.10)
project(logger_demo)  

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
  ${CMAKE_SOURCE_DIR}/Config/LoggerConfig.cpp  
  ${CMAKE_SOURCE_DIR}/Json/JsonUtils.cpp  
  ${CMAKE_SOURCE_DIR}/Logger/Logger.cpp 
  ${CMAKE_SOURCE_DIR}/main.cpp 
) 

find_package(Threads REQUIRED)
message(STATUS "${Threads}")

target_include_directories(logger PUBLIC 
  ${CMAKE_SOURCE_DIR}/3rdparty/json
  ${CMAKE_SOURCE_DIR}/src 
)

target_link_libraries(logger 
  PUBLIC ${CMAKE_THREAD_LIBS_INIT}
)
