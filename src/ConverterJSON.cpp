#include "../include/ConverterJSON.h"

std::vector<std::string> ConverterJSON::getTextDocuments()
{
    return textDocuments; 
}

int ConverterJSON::getResponsesLimit() const
{
    return maxResponses;
}

std::vector<std::string> ConverterJSON::getRequests()
{
    return requests;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
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
            nlohmann::json answerDictionary;
            for (auto request : answers)
            {
                answersDict["request" + std::to_string(requestCount)]["result"] = !request.empty();
                if (request.size() == 1)
                {
                    answersDict["request" + std::to_string(requestCount)]["docid"] = request[0].first;
                    answersDict["request" + std::to_string(requestCount)]["rank"] = request[0].second;
                }
                else
                {
                    auto relevance_array = nlohmann::json::array();
                    for (auto relevance : request)
                    {
                        auto relevance_member = nlohmann::json::object();
                        relevance_member["docid"] = relevance.first;
                        relevance_member["rank"] = relevance.second;
                        relevance_array.push_back(relevance_member);
                    }
                    answersDict["request" + std::to_string(requestCount)]["relevance"] = relevance_array;
                }
                ++requestCount;
            }
            answersFile << answersDict;
            answersFile.close();
        }
        else
        {
            std::cerr << "\t - file not found error: " + ANSWERS_FILE_PATH << "\n";
        }
        std::cout << "- - - - - - - - -\n";
    }
    else
    {
        std::cout << "No answers to push.\n";
    }
}

std::vector<std::string>* ConverterJSON::getFilesList()
{
    return &textDocuments;
}

void ConverterJSON::readConfigFile()
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
            textDocuments.push_back(f); //TODO: wrong realization - need to put in the CONTENT, not file names. 
            // я бы предложил перенести этот функционал в класс InvertedIndex, а здесь оставить только вывод списка файлов
            // из конфигурационного файла. 
        }

        //TODO: just for test (delete in release version)
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

void ConverterJSON::readRequestFile()
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

        //TODO: just for test (delete in release version)
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

bool ConverterJSON::clearFileContent(const std::string path)
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