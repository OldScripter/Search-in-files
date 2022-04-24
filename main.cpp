#include <iostream>
#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"

int main()
{
    std::cout << "Test\n";
    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();
    std::vector<std::string>* documents = ConverterJSON::getInstance()->getFilesList();
    InvertedIndex::getInstance()->updateDocumentBase(*documents);

    //Test:
    SearchServer searchServer(*InvertedIndex::getInstance());
    auto relevances = searchServer.search(ConverterJSON::getInstance()->getRequests());
    std::vector<std::vector<std::pair<int, float>>> answers;
    for (auto& relevancesGroup : relevances)
    {
        std::vector<std::pair<int, float>> answersGroup;
        for (auto& relevance : relevancesGroup)
        {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int) relevance.doc_id;
            relevancePair.second = relevance.rank;
            answersGroup.push_back(relevancePair);
        }
        answers.push_back(answersGroup);
    }
    ConverterJSON::getInstance()->putAnswers(answers);
    // TODO:ConverterJSON::getInstance()->putAnswers() - возможно станет ясно зачем нужен второй вектор.

    return 0;
}