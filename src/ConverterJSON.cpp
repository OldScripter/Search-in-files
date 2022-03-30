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
    return requests;
}

void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    nlohmann::json answersDict;
    if (!answers.empty())
    {
        std::cout << "Clearing answers file...";
        clearFileContent(ANSWERS_FILE_PATH) ? std::cout << "OK\n" : std::cout << "FAIL\n";

        std::cout << "Answers pushing...\n";
        std::ofstream answersFile(ANSWERS_FILE_PATH, std::ios_base::app);
        if (answersFile.is_open())
        {
            int requestCount = 0;
            int relevanceCount = 0;
            bool result = false;
            nlohmann::json answerDictionary;
            for (auto request : answers)
            {
                //записать request+id
                answersDict["request" + std::to_string(requestCount)]
                result = !request.empty(); // записать результат
                if (request.size() == 1)
                {
                    
                }
                else
                {
                    for (auto relevanse : request)
                    {
                        //записать пару
                    }
                }
                ++requestCount;
            }
            answersFile.close();
        }
        else
        {
            std::cerr << "\t - file not found error: " + ANSWERS_FILE_PATH << "\n";
        }
        std::cout << "- - - - - - - - -\n";
        //answersDict.push_back([])
    }
    else
    {
        std::cout << "No answers to push.\n"
    }
}

std::vector<std::string>* ConverterJSON::GetFilesList()
{
    return &textDocuments;
}

void ConverterJSON::ReadConfigFile()
{
    std::cout << "Configuration reading...\n";
    std::ifstream configFile(CONFIG_FILE_PATH);
    if (configFile.is_open())
    {
        nlohmann::json configDictionary;
        configFile >> configDictionary;
        applicationName = configDictionary["config"]["name"];
        applicationVersion = configDictionary["config"]["version"];
        maxResponses = configDictionary["config"]["max_responses"];
        textDocuments.clear();
        for (auto f : configDictionary["files"])
        {
            textDocuments.push_back(f);
        }

        //TODO: just for test
        for (auto file : textDocuments)
        {
            std::cout << file << "\n";
        }
        std::cout << applicationName << "\n";
        std::cout << "version: " << applicationVersion << "\n";
        std::cout << "max responses per request: " << maxResponses << "\n";
        //------------------------------------------------------------------
        configFile.close();
    }
    else
    {
        std::cerr << "\t - file not found error: " + CONFIG_FILE_PATH << "\n";
    }
    std::cout << "- - - - - - - - -\n";
}

void ConverterJSON::ReadRequestFile()
{
    std::cout << "Requests reading...\n";
    std::ifstream configFile(REQUEST_FILE_PATH);
    if (configFile.is_open())
    {
        nlohmann::json requestsDictionary;
        configFile >> requestsDictionary;
        requests.clear();
        for (auto f : requestsDictionary["requests"])
        {
            requests.push_back(f);
        }

        //TODO: just for test
        for (auto s : requests)
        {
            std::cout << s << "\n";
        }
        //------------------------------------------------------------------
        configFile.close();
    }
    else
    {
        std::cerr << "\t - file not found error: " + REQUEST_FILE_PATH << "\n";
    }
    std::cout << "- - - - - - - - -\n";
}

bool ConverterJSON::clearFileContent(std::string path)
{
    std::ofstream clearingStream(path, std::ios::trunc);
    if (clearingStream.is_open())
    {
        clearingStream.close();
        return true;
    }
    else
    {
        return false;
    }
}