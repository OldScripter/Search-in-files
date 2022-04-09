#pragma once
#include <iostream>
#include <vector>
#include <map>

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
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries_input);

    private:
    InvertedIndex _index;
    std::map<std::string, RelativeIndex> answer;
    
};