#include "../include/SearchServer.h"

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
        std::cerr << "Requests are empty.\n";
        return result;
    }

    for (const auto& query : queries_input)
    {
        // Get unique words from query
        std::set<std::string> uniqueWords = getUniqueWords(query);
        // TODO: Check unique words:
        {
            std::cout << "1. TEST - unique words:\n";
            for (auto word : uniqueWords) {
                std::cout << word << "\t";
            }
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------
        }


        // Get the entry count for each word
        auto wordsEntries = getWordsEntries(uniqueWords);
        // TODO: Check words entries:
        {
            std::cout << "2. TEST - wordsEntries:\n";
            for (auto entry : wordsEntries) {
                std::cout << "word: ";
                std::cout << entry.first << "\t";
                std::cout << "entries: ";
                std::cout << entry.second << "\n";
            }
            std::cout << "END OF TEST - - - - -\n";
            //---------------------------------------
        }

        // Sort unique words according to entry count in ascending direction
        sortWordsAscendingToEntries(wordsEntries);
        if (wordsEntries.back().second == 0)
        {
            std::cerr << "No matches are found.\n";
            return result;
        }
        // TODO: Check words entries:
        {
            std::cout << "3. TEST - wordsEntries after sort:\n";
            for (auto entry : wordsEntries) {
                std::cout << "word: ";
                std::cout << entry.first << "\t";
                std::cout << "entries: ";
                std::cout << entry.second << "\t";
            }
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------
        }

        // Get the document list of documents for each word
        // and get documents with all words in list as a vector:
        auto documentIds = getDocumentsWithAllWords(wordsEntries);
        // TODO: Check doc ids:
        {
            std::cout << "4. TEST - doc ids:\n";
            for (auto docId : documentIds) {
                std::cout << docId << "\t";
            }
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------
        }

        // Get absolute relevance and maximal relevance:
        std::vector<RelativeIndex> relativeIndexes;
        size_t maxRelevance {0};
        for (const auto& docId : documentIds)
        {
            size_t absoluteRelevance = getAbsoluteRelevanceForDocument(docId, uniqueWords);
            auto* relativeIndex = new RelativeIndex(docId, absoluteRelevance);

            relativeIndexes.push_back(*relativeIndex);
            if (absoluteRelevance > maxRelevance) maxRelevance = absoluteRelevance;
        }
        // TODO: MaxAbsRelevance:
        {
            std::cout << "5. TEST - max abs relevance:\n";
            std::cout << maxRelevance;
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------

            // TODO: Absolute indexes before sort:
            std::cout << "6. TEST - absolute relevances before sorting:\n";
            for (auto ind : relativeIndexes) {
                std::cout << "dicId: ";
                std::cout << ind.doc_id << "\t";
                std::cout << "abs_ind: ";
                std::cout << ind.absoluteIndex << "\t";
            }
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------
        }

        // Get relative relevance for each document:
        for (auto& relativeIndex : relativeIndexes)
        {
            relativeIndex.rank = (float) (relativeIndex.absoluteIndex) / (float) maxRelevance;
        }
                // TODO: Relative indexes before sort:
        {
            std::cout << "7. TEST - relative relevances before sorting:\n";
            for (auto ind : relativeIndexes) {
                std::cout << "dicId: ";
                std::cout << ind.doc_id << "\t";
                std::cout << "abs_ind: ";
                std::cout << ind.rank << "\t";
            }
            std::cout << "\nEND OF TEST - - - - -\n";
            //---------------------------------------
        }

        // Sort the documents according to relevance descending
        std::sort(relativeIndexes.begin(), relativeIndexes.end(), [&relativeIndexes](RelativeIndex &left, RelativeIndex &right)
        {
           return left.rank > right.rank;
        });

        // TODO: Relative indexes before sort:
        std::cout << "8. TEST - relative relevances after sorting:\n";
        for (auto ind : relativeIndexes)
        {
            std::cout << "dicId: ";
            std::cout << ind.doc_id << "\t";
            std::cout << "abs_ind: ";
            std::cout << ind.rank << "\t";
        }
        std::cout << "\nEND OF TEST - - - - -\n";
        //---------------------------------------

        // Push this vector to the result:
        result.push_back(relativeIndexes);
    }

    // TODO: check of search function result - delete in release -----------
    std::cout << "+ + + Returned from server.search:\n";
    for (auto answer : result)
    {
        for (auto subanswer : answer)
        {
            std::cout << "doc_id: ";
            std::cout << subanswer.doc_id << ";\t";
            std::cout << "abs_ind: ";
            std::cout << subanswer.absoluteIndex << ";\t";
            std::cout << "rel_ind: ";
            std::cout << subanswer.rank << "\n";
        }
        std::cout << "- - - - -\n";
    }
    std::cout << "\n";
    //---------------------------------------------------------------------

    // return the answer
    return result;
}