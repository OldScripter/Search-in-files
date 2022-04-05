cmake_minimum_required(VERSION 3.22)
project(SearchEngine)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)
set(CMAKE_CXX_STANDARD 14)

add_executable(SearchEngine main.cpp src/ConverterJSON.cpp)

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)
