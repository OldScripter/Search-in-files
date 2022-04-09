#include "../include/InvertedIndex.h"

//static members initialization -----------------------------

InvertedIndex* InvertedIndex::instance = nullptr;
std::map<int, std::string> InvertedIndex::document_list = {};
std::mutex InvertedIndex::mutexIndexMap;
std::vector<std::string> InvertedIndex::docs = {};
std::map<std::string, std::vector<Entry>> InvertedIndex::freq_dictionary = {};
bool InvertedIndex::indexing_is_ongoing = false;   
//-------------------------------------------------------------

InvertedIndex* InvertedIndex::getInstance()
{
    if (instance == nullptr)
    {
        instance = new InvertedIndex();
    }
    return instance;
}

void InvertedIndex::updateDocumentBase(std::vector<std::string> input_docs)
{
    // Update or fill docs vector.
    for (auto doc : input_docs)
    {
        std::ifstream doc_ifstream(doc);
        if (doc_ifstream.is_open())
        {
            std::string buffer;
            while (!doc_ifstream.eof())
            {
                std::string b;
                doc_ifstream >> b;
                buffer += b;
                buffer += " ";
            }
            
            std::cout << buffer << "\n";
            docs.push_back(buffer);
            doc_ifstream.close();
        }
        else
        {
            std::cerr << "File content reading:\t- file not found error: " + doc << "\n";
        }       
    }
        std::cout << "Input docs read success: " << docs.size() << " files\n";
        indexAllDocs();
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string& word)
{
    if (indexing_is_ongoing)
    {
        std::cout << "Index is ongoing, please repeat the request later.\n";
        return {};
    }
    auto it = freq_dictionary.find(word);
    if (it != freq_dictionary.end())
    {
        return it->second;
    }
    else
    {
        std::cout << "Word \"" << word << "\" not found.\n";
        return {};
    }
}

void InvertedIndex::indexTheFile(std::string fileContent, size_t docId)
{
    // Split doc on words.
    std::map<std::string, Entry> file_freq_dictionary;
    std::string word {""};
    Entry entry;
    entry.doc_id = docId;
    entry.count = 1;
    for (auto symbol : fileContent)
    {
        if (symbol == ' ' && fileContent.length())
        {
            std::pair<std::string, Entry> file_word_frequency {word, entry};
            // Insert or increment the word count
            if (!file_freq_dictionary.emplace(file_word_frequency).second)
            {
                file_freq_dictionary.find(word)->second.count += 1;
                std::cout << word << " : " << file_freq_dictionary.find(word)->second.count << "\n";
            }
            word = "";
        }
        else
        {
            word += symbol;
        }
    }
    // Place the word in map.
    mutexIndexMap.lock();
    std::cout << "Thread " << std::this_thread::get_id() << " map access is locked.\n";
    for (auto word_it : file_freq_dictionary)
    {
        std::pair<std::string, std::vector<Entry>> word_frequency;
        word_frequency.first = word_it.first;
        std::vector<Entry> entry_vector {word_it.second};
        word_frequency.second = entry_vector;
        if (!freq_dictionary.emplace(word_frequency).second)
        {
            freq_dictionary.find(word_frequency.first)->second.push_back(word_it.second);
        }
    }
    std::cout << "Thread " << std::this_thread::get_id() << " is finished.\n";
    std::cout << "Thread " << std::this_thread::get_id() << " map access is unlocked.\n";
    mutexIndexMap.unlock();

}

void InvertedIndex::indexAllDocs()
{
    if (docs.empty())
    {
        std::cerr << "\t- Indexing: no content in docs content base\n";
        return;
    }
    
    indexing_is_ongoing = true;

    freq_dictionary.clear();
    size_t doc_id = 0;
    for (auto content : docs)
    {
        // Start indexing thread.
        std::thread index(indexTheFile, content, doc_id);
        ++doc_id;
        std::cout << index.get_id() << " is joining\n";
        index.join();
    }
    indexing_is_ongoing = false;
    printTheFreqMap();
}