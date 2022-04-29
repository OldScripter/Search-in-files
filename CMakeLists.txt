cmake_minimum_required(VERSION 3.22)
project(Search_in_files)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

# Google test: ===================================================================
enable_testing()
# ================================================================================

add_subdirectory(nlohmann_json)
add_subdirectory(Google_tests)

add_executable(
        ${PROJECT_NAME}
        main.cpp
        src/ConverterJSON.cpp
        src/InvertedIndex.cpp
        src/SearchServer.cpp
        )

include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json ${CMAKE_EXE_LINKER_FLAGS})

# Copy auxiliary files to binary folder: ==========================================================
#[[file(GLOB RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.*")
file(COPY RESOURCES DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/requests.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/answers.json DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})]]

add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/config.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.json)
add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/requests.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/requests.json)
add_custom_command(
        TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/answers.json
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/answers.json)
add_custom_command(TARGET ${PROJECT_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources)
# =================================================================================================