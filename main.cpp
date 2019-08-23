#include <iostream>
#include "Parser.h"
#include "Query.h"

using namespace std;
int main() {
    std::cout << "Hello, World!" << std::endl;
    Parser* parser= new Parser();
    parser->parseFolder("../datasetsmall");
    Query* query = new Query();
    query->searchFile("../topics/topicssmall.txt", "indexToDocument.txt","invertedIndex.txt","avgAndNumDocuments.txt","documentLengths.txt");
    return 0;
}