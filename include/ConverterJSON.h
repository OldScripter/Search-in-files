#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include "nlohmann/json.hpp"

class ConverterJSON
{
    public:

    /**
    * Get the Instance object of ConverterJSON class
    * @return instance of the singleton
    */
    static ConverterJSON* getInstance();

    /**
     * Get the files content in string vector form
     * @return content of files listed in config.json
     */
    std::vector<std::string>* getTextDocuments();

    /**
     * Get the max responses per request
     * @return max responses count
     */
    int getResponsesLimit() const; // @StasTalanov: const is added (proposal) - not according to requirements.

    /**
     * Get the Requests from requests.json
     * @return requests vector
     */
    std::vector<std::string> getRequests();

    /**
     * Put requests results into answers.json file
     * @param[in] answers - vector of answers to be placed into answers.json file
     */
    void putAnswers(std::vector<std::vector<std::pair<int, float>>> answers);

    /**
     * Read config file specified in CONFIG_FILE_PATH
     */
    void readConfigFile();

    /**
     * Read request file specified in REQUEST_FILE_PATH
     */
    void readRequestFile();

    private:
    ConverterJSON() = default; // private constructor for singleton realization
    
    static ConverterJSON* instance;

    const std::string CONFIG_FILE_PATH = "..//config.json";     // TODO: Change in release version
    const std::string REQUEST_FILE_PATH = "..//requests.json";  // TODO: Change in release version
    const std::string ANSWERS_FILE_PATH = "..//answers.json";   // TODO: Change in release version

    std::string applicationName;
    std::string applicationVersion;
    int maxResponses;
    std::vector<std::string> resourcesPaths;
    std::vector<std::string> textDocuments;
    std::vector<std::string> requests;

    /**
     * Clear the file
     * @param [in] path - file path
     * @return true - if clearing is successful
     * @return false - if clearing is fail
     */
    bool clearFileContent(const std::string path);
};
