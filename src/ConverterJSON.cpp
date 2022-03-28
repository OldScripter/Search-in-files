#include "../include/ConverterJSON.h"

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    return textDocuments;
}

int ConverterJSON::GetResponsesLimit() const
{
    return maxResponses;
}

std::vector<std::string> ConverterJSON::GetRequests()
{

}

void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{

}

std::vector<std::string>* ConverterJSON::GetFilesList()
{
    return &textDocuments;
}

void ConverterJSON::ReadConfigFile()
{
    std::cout << "Converter JSON object creation\n";
    std::ifstream configFile(CONFIG_FILE_PATH);
    if (configFile.is_open())
    {
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        applicationName = configDictionary["config"]["name"];
        applicationVersion = configDictionary["config"]["version"];
        maxResponses = configDictionary["config"]["max_responses"];
        for (auto f : configDictionary["files"])
        {
            textDocuments.push_back(f);
        }

        std::cout << applicationName << "\n";
        std::cout << "version: " << applicationVersion << "\n";
        std::cout << "max responses per request: " << maxResponses << "\n";
        configFile.close();
    }
    else
    {
        std::cerr << "\t - file not found error: " + CONFIG_FILE_PATH << "\n";
    }
}