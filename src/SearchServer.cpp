#include "../include/SearchServer.h"

std::set<std::string> SearchServer::getUniqueWords(const std::string& request)
{
    std::set<std::string> result;
    std::istringstream ist(request);
    for (std::string word; ist >> word; )
    {
        //Convert symbols to lower case:
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        result.emplace(word);
    }
    /*for (char character :  request)
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
    }*/
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

std::vector<size_t> SearchServer::getDocumentsWithAllWords(const std::vector<std::pair<std::string, size_t>> &words)
{
    std::vector<size_t> docIds {};
    // Getting entries and docIds:
    for (const auto& wordAndEntry : words)
    {
        auto entries = InvertedIndex::getInstance()->getWordCount(wordAndEntry.first);
        for (auto entry : entries)
        {
            docIds.push_back(entry.doc_id);
        }
    }

    // Clearing doc ids where at least 1 word is missing:
    docIds.erase(std::remove_if(docIds.begin(), docIds.end(), [&docIds, &words](const size_t &x)
    {
        return std::count(docIds.begin(), docIds.end(), x) != words.size();
    }), docIds.end());

    // Getting unique ids from docIds:
    std::set<size_t> uniqueDocIds (docIds.begin(), docIds.end());
    docIds.clear();
    docIds.assign(uniqueDocIds.begin(), uniqueDocIds.end());
    return docIds;
}

void SearchServer::sortWordsAscendingToEntries(std::vector<std::pair<std::string, size_t>>& wordsEntries)
{
    std::sort(wordsEntries.begin(), wordsEntries.end(), [](auto &left, auto &right)
    {
        return left.second < right.second;
    });
}

size_t SearchServer::getAbsoluteRelevanceForDocument(size_t docId, std::set<std::string> &uniqueWords) {
   size_t absoluteRelevance {0};
   for (const auto& word : uniqueWords)
   {
       absoluteRelevance += InvertedIndex::getInstance()->getWordCountInDoc(word, docId);
   }
    return absoluteRelevance;
}

std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries_input)
{
    std::vector<std::vector<RelativeIndex>> result{};
    if (queries_input.empty())
    {
        std::cout << "Requests are empty.\n";
        return result;
    }

    for (const auto& query : queries_input)
    {
        // Get unique words from query
        std::set<std::string> uniqueWords = getUniqueWords(query);
        if (uniqueWords.empty())
        {
            std::cout << "\t-bad request.\n";
            continue;
        }

        // Get the entry count for each word
        auto wordsEntries = getWordsEntries(uniqueWords);

        // Sort unique words according to entry count in ascending direction
        sortWordsAscendingToEntries(wordsEntries);

        // Get the document list of documents
        auto documentIds = getDocumentsWithAllWords(wordsEntries);
        std::string docOrDocs = documentIds.size() == 1 ? " document " : " documents ";
        std::string wordOrWords = uniqueWords.size() == 1 ? " word: " : " words: ";
        std::cout << "\t-found " << documentIds.size() << docOrDocs << "with" << wordOrWords;
        for (auto word : uniqueWords)
        {
            std::cout << word << " ";
        }
        std::cout << "\n";

        // Get absolute relevance and maximal relevance:
        std::vector<RelativeIndex>* relativeIndexes = new std::vector<RelativeIndex>();
        size_t maxRelevance {0};
        for (const auto& docId : documentIds)
        {
            size_t absoluteRelevance = getAbsoluteRelevanceForDocument(docId, uniqueWords);
            auto* relativeIndex = new RelativeIndex();
            relativeIndex->doc_id = docId;
            relativeIndex->absoluteIndex = absoluteRelevance;

            relativeIndexes->push_back(*relativeIndex);
            if (absoluteRelevance > maxRelevance) maxRelevance = absoluteRelevance;
        }

        // Get relative relevance for each document:
        for (auto& relativeIndex : *relativeIndexes)
        {
            if (maxRelevance != 0) relativeIndex.rank = (float) (relativeIndex.absoluteIndex) / (float) maxRelevance;
            else relativeIndex.rank = 0;
        }

        // Sort the documents according to relevance descending
        std::sort(relativeIndexes->begin(), relativeIndexes->end(), [&relativeIndexes](RelativeIndex &left, RelativeIndex &right)
        {
           return left.rank > right.rank;
        });

        // Push this vector to the result:
        result.push_back(*relativeIndexes);
    }
    return result;
}