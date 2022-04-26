cmake_minimum_required(VERSION 3.22)
project(Search_in_files_v2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

# Google test: ===================================================================
include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
# ================================================================================

add_subdirectory(nlohmann_json)

add_executable(
        Search_in_files_v2
        main.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        src/Test.cpp)

target_link_libraries(Search_in_files_v2 PRIVATE nlohmann_json::nlohmann_json gtest_main ${CMAKE_EXE_LINKER_FLAGS})
include(GoogleTest)
gtest_discover_tests(Search_in_files_v2)