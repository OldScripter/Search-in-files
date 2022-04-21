cmake_minimum_required(VERSION 3.22)
project(Search_in_files_v2)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(nlohmann_json)

add_executable(
        Search_in_files_v2
        main.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        )

target_link_libraries(Search_in_files_v2 PRIVATE nlohmann_json::nlohmann_json)