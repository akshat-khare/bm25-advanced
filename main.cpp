#include <iostream>
#include "Parser.h"
#include "Query.h"

using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
//    Parser* parser= new Parser();
//    parser->parseFolder("../dataset");
    Query* query = new Query();
    query->searchFile("../topics/topics.txt", "indexToDocument.txt","invertedIndex.txt","avgAndNumDocuments.txt","documentLengths.txt");
    return 0;
}