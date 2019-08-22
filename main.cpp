#include <iostream>
#include "Parser.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    Parser parser;
    parser.parseFolder("dataset");

    return 0;
}