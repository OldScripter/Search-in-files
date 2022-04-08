#include <iostream>
#include "include/ConverterJSON.h"
#include "include/InvertedIndex.h"
#include "gtest/gtest.h"


TEST(sample_test_case, sample_test)
{
    EXPECT_EQ(1, 1);
}

int main(int argc, char* argv[])
{
    std::cout << "Test\n";
    ConverterJSON converterJSON;
    converterJSON.readConfigFile();
    converterJSON.readRequestFile();
    std::vector<std::string>* documents = converterJSON.getFilesList();
    InvertedIndex invertedIndex;
    invertedIndex.updateDocumentBase(*documents);
    


    /* std::vector<std::vector<std::pair<int, float>>> answers;
    std::vector<std::pair<int, float>> relevance1;
    std::pair<int, float> p11 = {1, 1.1f};
    std::pair<int, float> p12 = {2, 1.2f};
    std::pair<int, float> p13 = {3, 1.3f};
    std::pair<int, float> p14 = {4, 1.4f};
    relevance1.push_back(p11);
    relevance1.push_back(p12);
    relevance1.push_back(p13);
    relevance1.push_back(p14);
    std::vector<std::pair<int, float>> relevance2;
    std::pair<int, float> p21 = {1, 2.1f};
    relevance2.push_back(p21);
    std::vector<std::pair<int, float>> relevance3;
    answers.push_back(relevance1);
    answers.push_back(relevance2);
    answers.push_back(relevance3);

    converterJSON.putAnswers(answers); */

    return 0;
}