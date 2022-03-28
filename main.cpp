#include <iostream>
#include "include/ConverterJSON.h"

int main(int argc, char* argv[])
{
    std::cout << "Test\n";
    ConverterJSON converterJSON;
    converterJSON.ReadConfigFile();
    std::vector<std::string>* f = converterJSON.GetFilesList();
    //Check the vector:
    for (auto file : *f)
    {
        std::cout << file << "\n";
    }

    return 0;
}