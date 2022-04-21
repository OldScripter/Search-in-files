#include <iostream>
#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "include/SearchServer.h"

int main()
{
    std::cout << "Test\n";
    ConverterJSON::getInstance()->readConfigFile();
    ConverterJSON::getInstance()->readRequestFile();
    std::vector<std::string>* documents = ConverterJSON::getInstance()->getFilesList();
    InvertedIndex::getInstance()->updateDocumentBase(*documents);

    //Test:
    SearchServer searchServer(*InvertedIndex::getInstance());
    auto res = searchServer.search(ConverterJSON::getInstance()->getRequests());
    // TODO:ConverterJSON::getInstance()->putAnswers() - возможно станет ясно зачем нужен второй вектор.

    return 0;
}