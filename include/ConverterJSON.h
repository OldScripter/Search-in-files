#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

class ConverterJSON
{
    public:
    /**
     * @brief Construct a new Converter JSON object
     */
    ConverterJSON() = default;

    /**
     * @brief Get the files content in string vector form
     * @return std::vector<std::string> content of files listed in config.json
     */
    std::vector<std::string> GetTextDocuments();

    /**
     * @brief Get the max responses per request 
     * @return int max responses count
     */
    int GetResponsesLimit() const; // @StasTalanov: const is added (proposal) - not according to requirements.

    /**
     * @brief Get the Requests from requests.json
     * @return std::vector<std::string> requests list
     */
    std::vector<std::string> GetRequests();

    /**
     * @brief Put requests results into answers.json file
     * @param[in] answers list of answers to be placed into answers.json file
     */
    void PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

    /**
     * @brief Read config file specified in CONFIG_FILE_PATH
     */
    void ReadConfigFile();

    /**
     * @brief Read request file specified in REQUEST_FILE_PATH
     */
    void ReadRequestFile();

    /**
     * @brief Get the Files List specified in config.json
     * @return std::vector<std::string>* pointer for the files list
     */
    std::vector<std::string>* GetFilesList();

    private:

    const std::string CONFIG_FILE_PATH = "..//config.json";     // TODO: Change in release version
    const std::string REQUEST_FILE_PATH = "..//requests.json";  // TODO: Change in release version
    const std::string ANSWERS_FILE_PATH = "..//answers.json";   // TODO: Change in release version

    std::string applicationName;
    std::string applicationVersion;
    int maxResponses;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;

    /**
     * @brief Clear the file
     * 
     * @param path [in] - as file path
     * @return true - if clearing is successful
     * @return false if celaring is fail
     */
    bool clearFileContent(const std::string path);
};
