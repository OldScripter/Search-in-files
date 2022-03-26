cmake_minimum_required(VERSION 3.0.0)
project(search-in-files VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")


include(CTest)
enable_testing()

add_subdirectory(nlohmann_json)
add_executable(search-in-files main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(search-in-files PRIVATE nlohmann_json::nlohmann_json)