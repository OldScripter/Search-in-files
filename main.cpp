#include <iostream>
#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"

/**
 * Write answers to the JSON file answers.json
 * @param [in] allRequestsResults result of search for relevant pages
 */
void writeAnswers(const std::vector<std::vector<RelativeIndex>>& allRequestsResults)
{
    if (allRequestsResults.empty())
    {
        std::cout << "No matches are found.\n";
        return;
    }
    std::vector<std::vector<std::pair<int, float>>> allRequestsResultsReadyForJSON;
    for (auto& requestResult : allRequestsResults)
    {
        std::vector<std::pair<int, float>> requestResultReadyForJSON;
        for (auto& pageRelevance : requestResult)
        {
            std::pair<int, float> relevancePair;
            relevancePair.first = (int) pageRelevance.doc_id;
            relevancePair.second = pageRelevance.rank;
            requestResultReadyForJSON.push_back(relevancePair);
        }
        allRequestsResultsReadyForJSON.push_back(requestResultReadyForJSON);
    }
    ConverterJSON::getInstance()->putAnswers(allRequestsResultsReadyForJSON);
}


int main()
{
    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();
    std::vector<std::string>* documents = ConverterJSON::getInstance()->getFilesList();
    InvertedIndex::getInstance()->updateDocumentBase(*documents);

    std::cout << "Searching...\n";
    SearchServer searchServer(*InvertedIndex::getInstance());
    auto allRequestsResults = searchServer.search(ConverterJSON::getInstance()->getRequests());
    writeAnswers(allRequestsResults);
    std::cout << "End of search.\n";

    return 0;
}