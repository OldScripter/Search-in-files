#include "../include/SearchServer.h"

void SearchServer::printSet(std::set<std::string>& container)
{
    std::cout << "\t- Debug: ";
    if (container.empty())
    {
        std::cout << "Set is empty\n";
        return;
    }
    std::cout << "Set: ";
    for (auto const &value: container)
    {
        std::cout << value << " ";
    }
    std::cout << "\n";
}

void SearchServer::printVector(std::vector<std::pair<std::string, size_t>>& container)
{
    std::cout << "\t- Debug: ";
    if (container.empty())
    {
        std::cout << "Vector is empty\n";
        return;
    }
    std::cout << "Vector: ";
    for (auto const &value: container)
    {
        std::cout << value.first << "-" << value.second << " ";
    }
    std::cout << "\n";
}

std::set<std::string> SearchServer::getUniqueWords(const std::string& request)
{
    std::set<std::string> result;
    std::string word;
    for (char character :  request)
    {
        if (character != ' ')
        {
            word += character;
        }
        else
        {
            if (!word.empty()) result.emplace(word);
            word = "";
        }
    }
    if (!word.empty()) result.emplace(word);
    return result;
}

std::vector<std::pair<std::string, size_t>> SearchServer::getWordsEntries(const std::set<std::string>& words)
{
    std::vector<std::pair<std::string, size_t>> result;
    for (const auto& word : words)
    {
        auto wordEntries = InvertedIndex::getInstance()->getWordCount(word);
        size_t wordEntriesSum = 0;
        for (auto wordEntry : wordEntries)
        {
            wordEntriesSum += wordEntry.count;
        }
        std::pair<std::string, size_t> wordAndEntry;
        wordAndEntry.first = word;
        wordAndEntry.second = wordEntriesSum;
        result.push_back(wordAndEntry);
    }
    return result;
}

std::vector<size_t> SearchServer::getDocumentsWithAllWords(const std::vector<std::pair<std::string, size_t>> &words) {
    std::vector<size_t> result;
    for (auto wordAndEntry : words)
    {
        auto entries = InvertedIndex::getInstance()->getWordCount(wordAndEntry.first);
        //TODO: check the entries list.
        for (auto entry : entries)
        {
            result.push_back(entry.doc_id);
        }
    }

    //Clearing doc ids where at least 1 word is missing:
    result.erase(std::remove_if(result.begin(), result.end(), [&result, &words](const size_t &x)
    {
        return std::count(result.begin(), result.end(), x) != words.size();
    }), result.end());

    return result;
}


void SearchServer::sortWordsAscendingToEntries(std::vector<std::pair<std::string, size_t>>& wordsEntries)
{
    std::sort(wordsEntries.begin(), wordsEntries.end(), [](auto &left, auto &right)
    {
        return left.second < right.second;
    });
}

void SearchServer::printDocumentIds(std::vector<size_t>& documentIds)
{
    std::cout << "Doc ids:\n";
    for (auto docId : documentIds)
    {
        std::cout << docId << " ";
    }
    std::cout << "\n";
}


std::vector<RelativeIndex> SearchServer::search(const std::vector<std::string>& queries_input)
{
    for (const auto& query : queries_input)
    {
        std::cout << "\t- Debug: The request is: " << query << " ";
        std::cout << "\n";

        // Get unique words from query
        std::set<std::string> uniqueWords = getUniqueWords(query);
        printSet(uniqueWords);

        // Get the entry count for each word
        auto wordsEntries = getWordsEntries(uniqueWords);
        printVector(wordsEntries);

        // Sort unique words according to entry count in ascending direction
        sortWordsAscendingToEntries(wordsEntries);
        printVector(wordsEntries);

        // Get the document list of documents for each word
        // Get documents with all words in list as a vector
        auto documentIds = getDocumentsWithAllWords(wordsEntries);
        printDocumentIds(documentIds);

        // for each document
        // {
            // Get document absolute relevance
        // }
        // for each document
        // {
            // Get document relative relevance
        // }
        // Sort the documents according to relevance descending
        // Push this vector to the result
    }
    // return the answer
    std::vector<RelativeIndex> res = {};
    return res;
}








/*

std::vector<RelativeIndex> SearchServer::search(const std::vector<std::string>& queries_input)
{

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

    std::vector<std::pair<std::string, int>> wordsEntryVector;
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

    // Find all docs in which each word appears:
    std::vector <size_t> doc_ids;

    // Получаем перечень файлов в которых встречается самое редкое слово и добавляем в вектор doc_ids.
    auto entries = _index.getWordCount(wordsEntryVector[0].first);
    // Добавляем все doc_id в doc_ids:
    for (auto entry : entries)
    {
        doc_ids.push_back(entry.doc_id);
    }
    // Тест - печатаем doc_ids до чистки
    for (auto doc_id : doc_ids)
    {
        std::cout << doc_id << " ";
    }

    // Проверяем в каких файлах встречаются другие слова (нужен рефакторинг - можно проще).
    for (int i = 1; i < wordsEntryVector.size(); ++i)
    {
        auto other_entries = _index.getWordCount(wordsEntryVector[i].first);
        //Проходимся по doc_ids: если в other_entries не содержаться все doc_id из doc_ids - удаляем лишние из doc_ids.
        for (auto doc_id : doc_ids)
        {
            bool isFound = false;
            for (auto entry : other_entries)
            {
                if (entry.doc_id == doc_id)
                {
                    isFound = true;
                    break;
                }
            }
            if (!isFound)
            {
                auto after_removal (std::remove(begin(doc_ids), end(doc_ids), doc_id));
                doc_ids.erase(after_removal, end(doc_ids));
                doc_ids.shrink_to_fit();
            }
        }
    }

    // Печатаем doc_ids - для теста:
    std::cout << "\n";
    for (auto doc_id : doc_ids)
    {
        std::cout << doc_id << " ";
    }
    std::cout << "\n";

    // Calculating absolute indexes and max absolute index:
    std::vector<size_t> absoluteIndexes;
    size_t maxAbsoluteIndex = 0;
    for (auto doc_id : doc_ids)
    {
        size_t absoluteIndex = 0;
        for (const auto& wordsEntry : wordsEntryVector)
        {
            auto word = wordsEntry.first;
            absoluteIndex += _index.getWordCountInDoc(word, doc_id);
            maxAbsoluteIndex = absoluteIndex > maxAbsoluteIndex ? absoluteIndex : maxAbsoluteIndex;
        }
        absoluteIndexes.push_back(absoluteIndex);
    }

    // Print absolute indexes:
    std::cout << "Unsorted answer:\n";
    for (auto value : absoluteIndexes)
    {
        std::cout << "abindex " << value << " : maxindex " << maxAbsoluteIndex << "\n";
    }

    //Calculating relevance indexes:
    std::vector<RelativeIndex> answer{};

    if (absoluteIndexes.empty()) return answer;

    for (size_t i = 0; i < absoluteIndexes.size(); ++i)
    {
        RelativeIndex relativeIndex{};
        relativeIndex.doc_id = absoluteIndexes.size() - i - 1;
        relativeIndex.rank = (float)absoluteIndexes[i] / (float)maxAbsoluteIndex;
        answer.push_back(relativeIndex);
    }

    // Print unsorted answer:
    std::cout << "Unsorted answer:\n";
    for (auto value : answer)
    {
        std::cout << "doc " << value.doc_id << " : rank " << value.rank << "\n";
    }

    std::sort(answer.begin(), answer.end(), [](RelativeIndex &left, RelativeIndex &right)
    {
        return left.rank > right.rank;
    });

    // Print sorted answer:
    std::cout << "Sorted answer:\n";
    for (auto value : answer)
    {
        std::cout << "doc " << value.doc_id << " : rank " << value.rank << "\n";
    }

return answer;

}

*/

