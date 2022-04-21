#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

#include "InvertedIndex.h"

struct RelativeIndex
{
    size_t doc_id;
    float rank;
    bool operator ==(const RelativeIndex& other) const 
    {
        return (doc_id == other.doc_id && rank == other.rank);
    }
};
class SearchServer 
{
    public:
    
    /**
    * @param idx link to the InvertedIndex instance to
    * let SearchServer know the word frequency in files.
    */
    SearchServer(InvertedIndex& idx) : _index(idx){ };
    /**
    * Process the search request
    * @param queries_input search requests from requests.json
    * @return the sorted list of relevant answers
    */
    std::vector<RelativeIndex> search(const std::vector<std::string>& queries_input);

    /*
     * В ТЗ было std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);
     * Но мне кажется что еще один std::vector в типе возвращаемого значения лишний
     * просто std::vector<RelativeIndex> - достаточно.
     */

    private:
    InvertedIndex _index;
    //std::map<std::string, RelativeIndex> answer;

    std::set<std::string> getUniqueWords(const std::string& request);
    void printSet(std::set<std::string>& container); //TODO: Delete in release
    std::vector<std::pair<std::string, size_t>> getWordsEntries(const std::set<std::string>& words);
    void printVector(std::vector<std::pair<std::string, size_t>>& container); //TODO: Delete in release
    void sortWordsAscendingToEntries(std::vector<std::pair<std::string, size_t>>& wordsEntries);
    std::vector<size_t> getDocumentsWithAllWords(const std::vector<std::pair<std::string, size_t>>& words);
    void printDocumentIds(std::vector<size_t>& documentIds);
};