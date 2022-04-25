#include "../include/ConverterJSON.h"

const int LINE_LENGTH = 46;
const int HEADER_SPACER = 15;

//static members initialization -------------------------
ConverterJSON* ConverterJSON::instance = nullptr;
ConverterJSON* ConverterJSON::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ConverterJSON();
    }
    return instance;
}
//---------------------------------------------------------

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
        std::cout << "Clearing answers file... ";
        clearFileContent(ANSWERS_FILE_PATH) ? std::cout << "done\n" : std::cout << "error\n";

        std::cout << "Answers pushing... ";
        std::ofstream answersFile(ANSWERS_FILE_PATH, std::ios_base::app);
        if (answersFile.is_open())
        {
            int requestCount {0};
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
                    int relevanceID{0};
                    for (auto relevance : request)
                    {
                        ++relevanceID;
                        if (relevanceID > maxResponses) break;

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
            std::cout << "done\n";
        }
        else
        {
            std::cout << "\t error - file not found: " + ANSWERS_FILE_PATH << "\n";
        }
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

        for (int i = 0; i < HEADER_SPACER; ++i) {std::cout << "=";}
        std::cout << "[Initialization]";
        for (int i = 0; i < HEADER_SPACER; ++i) {std::cout << "=";}
        std::cout << "\n" << applicationName << "\n";
        std::cout << "Version: " << applicationVersion << "\n";
        std::cout << "Max responses per request: " << maxResponses << "\n";
        std::cout << "Files library: " << textDocuments.size() << "\n";
        for (int i = 0; i < LINE_LENGTH; ++i) {std::cout << "-";}
        std::cout << "\n";
        configFile.close();
    }
    else
    {
        std::cerr << "\t - file not found error: " + CONFIG_FILE_PATH << "\n";
    }
}

void ConverterJSON::readRequestFile()
{
    std::cout << "Requests reading: ";
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
        configFile.close();
        std::string requestOrRequests = requests.size() == 1 ? " request is " : " requests are ";
        std::cout << requests.size() << requestOrRequests << "found\n";
    }
    else
    {
        std::cerr << "\t - file not found error: " + REQUEST_FILE_PATH << "\n";
    }
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