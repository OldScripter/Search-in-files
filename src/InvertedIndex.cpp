#include "InvertedIndex.h"

const int HEADER_SPACER = 15;

//static members initialization -----------------------------
InvertedIndex* InvertedIndex::instance {nullptr};
std::map<int, std::string> InvertedIndex::document_list = {};
std::mutex InvertedIndex::mutexIndexMap;
std::map<std::string, std::vector<Entry>> InvertedIndex::frequencyDictionary = {};
bool InvertedIndex::indexingIsOngoing = false;
//-------------------------------------------------------------

InvertedIndex* InvertedIndex::getInstance()
{
    if (instance == nullptr)
    {
        instance = new InvertedIndex();
    }
    return instance;
}

void InvertedIndex::updateDocumentBase(const std::vector<std::string>& input_docs)
{
    if (input_docs.empty())
    {
        std::cerr << "\t- Indexing: no content in docs content base\n";
        return;
    }

    indexingIsOngoing = true;

    frequencyDictionary.clear();
    size_t docId = 0;
    for (const auto& content : input_docs)
    {
        // Start indexing thread.
        std::thread index(indexTheFile, content, docId);
        ++docId;
        index.join();
    }
    indexingIsOngoing = false;
    //Uncomment for debug: std::cout << "Frequency map:\n";
    //Uncomment for debug: printTheFreqMap();
    //Uncomment for debug: for (int i = 0; i < HEADER_SPACER; ++i) {std::cout << "=";}
    //Uncomment for debug: std::cout << "[Ready for search]";
    //Uncomment for debug: for (int i = 0; i < HEADER_SPACER; ++i) {std::cout << "=";}
    //Uncomment for debug: std::cout << "\n";
}

std::vector<Entry> InvertedIndex::getWordCount(const std::string& word)
{
    if (indexingIsOngoing)
    {
        std::cout << "Index is ongoing, please repeat the request later.\n";
        return {};
    }
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end())
    {
        return it->second;
    }
    else
    {
        return {};
    }
}

void InvertedIndex::indexTheFile(const std::string& fileContent, size_t docId)
{
    // Split doc on words.
    std::map<std::string, Entry> fileFreqDictionary;
    Entry entry{};
    entry.doc_id = docId;
    entry.count = 1;
    std::istringstream ist(fileContent);
    for (std::string word; ist >> word; )
    {
        //Convert symbols to lower case:
        std::transform(word.begin(), word.end(), word.begin(),
                       [](unsigned char c){ return std::tolower(c); });

        std::pair<std::string, Entry> file_word_frequency {word, entry};
        if (!fileFreqDictionary.emplace(file_word_frequency).second)
        {
            fileFreqDictionary.find(word)->second.count += 1;
        }
    }
    // Place the word in map.
    mutexIndexMap.lock();
    for (const auto& wordIterator : fileFreqDictionary)
    {
        std::pair<std::string, std::vector<Entry>> wordFrequency;
        wordFrequency.first = wordIterator.first;
        std::vector<Entry> entryVector {wordIterator.second};
        wordFrequency.second = entryVector;
        if (!frequencyDictionary.emplace(wordFrequency).second)
        {
            frequencyDictionary.find(wordFrequency.first)->second.push_back(wordIterator.second);
        }
    }
    mutexIndexMap.unlock();
}

size_t InvertedIndex::getWordCountInDoc(const std::string& word, const size_t doc_id) const
{
    if (indexingIsOngoing) {
        std::cout << "Index is ongoing, please repeat the request later.\n";
        return 0;
    }
    size_t wordCountInDoc = 0;
    auto it = frequencyDictionary.find(word);
    if (it != frequencyDictionary.end()) {
        auto entryVector = it->second;
        for (auto entry : entryVector) {
            if (entry.doc_id == doc_id) wordCountInDoc += entry.count;
        }
    } else {
        std::cout << "Word \"" << word << "\" not found.\n";
    }
    return wordCountInDoc;
}

void InvertedIndex::printTheFreqMap()
{
    if (frequencyDictionary.empty()) std::cerr << "Empty map!\n";

    for (const auto& word : frequencyDictionary)
    {
        std::cout << word.first << ":\n";
        for (auto freq : word.second)
        {
            std::cout << "\t- " << freq.doc_id << ", " << freq.count << "\n";
        }
    }
}
