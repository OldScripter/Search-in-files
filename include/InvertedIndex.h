#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <sstream>

struct Entry 
{
    size_t doc_id, count;

    // Required for tests:
    bool operator ==(const Entry& other) const 
    {
        return (doc_id == other.doc_id && count == other.count);
    }
};

class InvertedIndex 
{
    public:
    InvertedIndex() = default;

    /**
    * Update or fill the documents base in which search will be performed
    * @param texts_input documents content
    */
    void updateDocumentBase(std::vector<std::string> input_docs);
    
    /**
    * Get the count of \'word\' entries in ready documents base
    * @param word the word for which the entry frequency should be detected
    * @return the list of words with calculated entry frequency
    */
    std::vector<Entry> getWordCount(const std::string& word);

    private:
    std::vector<std::string> docs; // list of documents content
    std::map<int, std::string> document_list; // map of documents (unique id and file name) for search in 
    std::map<std::string, std::vector<Entry>> freq_dictionary; // frequency dictionary for all files
    std::vector<std::thread> threads; // vector for thread storing
    std::mutex mutexIndexMap; // access to index map
    size_t docs_count; // to be used for detection if indexing is finished
    bool indexing_is_ongoing = false;

    void indexTheFile(std::string fileContent, size_t docId);

    /**
     * @brief Start indexing of all files in separate threads
     */
    void indexAllDocs();

    // Just for test:
    void printTheFreqMap()
    {
        if (freq_dictionary.empty()) std::cerr << "Empty map!\n";

        for (auto word : freq_dictionary)
        {
            std::cout << word.first << ":\n";
            for (auto freq : word.second)
            {
                std::cout << "\t- " << freq.doc_id << ", " << freq.count << "\n";
            }
        }
    }
};