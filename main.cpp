#include <iostream>
#include "include/ConverterJSON.h"

int main(int argc, char* argv[])
{
    std::cout << "Test\n";
    ConverterJSON converterJSON;
    converterJSON.ReadConfigFile();
    converterJSON.ReadRequestFile();
    std::vector<std::string>* f = converterJSON.GetFilesList();


    return 0;
}