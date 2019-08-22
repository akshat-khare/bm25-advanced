#include <iostream>
#include "Parser.h"
using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    Parser* parser= new Parser();
    parser->parseFolder("../dataset");

    return 0;
}