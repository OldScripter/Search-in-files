#include "../include/ConverterJSON.h"

ConverterJSON::ConverterJSON()
{
    std::cout << "Converter JSON object creation\n";
}

std::vector<std::string> ConverterJSON::GetTextDocuments()
{
    
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