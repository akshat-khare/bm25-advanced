//
// Created by codebat on 23/08/19.
//
#include <iostream>
#include "Parser.h"

using namespace std;
int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    Parser* parser= new Parser();
    parser->parseFolder(argv[2], argv[4]);
    return 0;
}
