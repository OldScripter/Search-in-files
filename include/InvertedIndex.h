#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>

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

    /**
     * Get the Instance object of InvertedIndex class
     * @return instance of the singleton
     */
    static InvertedIndex* getInstance();

    /**
    * Update or fill the documents base in which search will be performed
    * @param [in] texts_input - documents content
    */
    void updateDocumentBase(const std::vector<std::string>& input_docs);
    
    /**
    * Get the count of \'word\' entries in ready documents base
    * @param [in] word - word for which the entry frequency should be detected
    * @return vector of words with calculated entry frequency
    */
    std::vector<Entry> getWordCount(const std::string& word);

    /**
     * Get the word count in certain document
     * @param [in] word - word to counted in a document
     * @param [in] doc_id - document id for search
     * @return count of the word in certain document
     */
    size_t getWordCountInDoc(const std::string& word, const size_t doc_id) const; // ADDITIONAL METHOD

    /**
     * Start indexing of all files in separate threads
     */
    void indexAllDocs();

    private:
    
    InvertedIndex() = default; //private constructor for singleton implementation
    
    /**
     * Perform the indexing of the separate file
     * @param [in] fileContent - std::string with file content
     * @param [in] docId - id of the file
     */
    static void indexTheFile(std::string fileContent, size_t docId);

    static InvertedIndex* instance;
    static std::map<int, std::string> document_list; // map of documents (unique id and file name) for search in
    static std::mutex mutexIndexMap; // access to index map
    static std::vector<std::string> docs; // list of documents content
    static std::map<std::string, std::vector<Entry>> frequencyDictionary; // frequency dictionary for all files
    static bool indexingIsOngoing;

    // Just for test
    void printTheFreqMap()
    {
        if (frequencyDictionary.empty()) std::cerr << "Empty map!\n";

        for (auto word : frequencyDictionary)
        {
            std::cout << word.first << ":\n";
            for (auto freq : word.second)
            {
                std::cout << "\t- " << freq.doc_id << ", " << freq.count << "\n";
            }
        }
    }
};