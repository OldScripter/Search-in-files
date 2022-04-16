cmake_minimum_required(VERSION 3.0.0)
project(search-in-files VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")


include(CTest)
include(FetchContent)
FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

enable_testing()

add_subdirectory(nlohmann_json)
add_executable(search-in-files 
main.cpp 
src/ConverterJSON.cpp
src/InvertedIndex.cpp
src/SearchServer.cpp
)

#[[set(gtest_disable_pthreads on) use for MinGW ]]
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_link_libraries(search-in-files PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(search-in-files PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search-in-files)