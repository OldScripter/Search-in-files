#pragma once
#include <iostream>
#include <vector>

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

    private:

    std::string applicationName;
    std::string applicationVersion;
    int maxResponses;

    bool readConfigurationFile();
    std::string getApplicationName() const;
    std::string getApplicationVersion() const;
};
