#include "../include/SearchServer.h"

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::pair<std::string, int>> wordsEntryVector;
    std::map<std::string, int> wordsEntryMap;
    // Get the uniques words from input
    for (auto word : queries_input)
    {
        std::pair<std::string, RelativeIndex> result;
        auto entries = _index.getWordCount(word);
        auto entriesCount = 0;
        for (auto entry : entries)
        {
            entriesCount += entry.count;
        }
        std::pair<std::string, int> wordToEmplace (word, entriesCount);
        auto emplace_result = wordsEntryMap.emplace(wordToEmplace);
    }

    // Upload unique values to vector:

    for (auto wordsEntryPair : wordsEntryMap)
    {
        wordsEntryVector.push_back(wordsEntryPair);
    }

    // Print words & etries vector:

    for (auto wordsEntryPair : wordsEntryVector)
    {
        std::cout << wordsEntryPair.first << " : " << wordsEntryPair.second << "\n";
    }
    
    // Sort the vector according to entriesCount:

    std::sort(wordsEntryVector.begin(), wordsEntryVector.end(), [](auto &left, auto &right) 
    {
        return left.second < right.second;
    });

    // Print words & etries vector after sorting:

    for (auto wordsEntryPair : wordsEntryVector)
    {
        std::cout << wordsEntryPair.first << " : " << wordsEntryPair.second << "\n";
    }

    // Find all docs in which each word appers:
    std::vector<Entry> v = _index.getWordCount(wordsEntryVector[0].first);
    for (auto wordsSortedEntryPair : wordsEntryVector)
    {
        

    }

    RelativeIndex relativeIndex;
    relativeIndex.doc_id = 1;
    relativeIndex.rank = 10;
    std::vector<RelativeIndex> testVector;
    testVector.push_back(relativeIndex);
    std::vector<std::vector<RelativeIndex>> answer;
    answer.push_back(testVector);
    return answer;
}


