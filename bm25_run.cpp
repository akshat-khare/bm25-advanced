//
// Created by codebat on 23/08/19.
//
#include <iostream>
#include "Query.h"

using namespace std;
int main(int argc, char* argv[]) {
    std::cout << "Hello, World!" << std::endl;
    Query* query = new Query();
    query->searchFile(argv[4], "_indexToDocument.txt","_invertedIndex.txt","_avgAndNumDocuments.txt","_documentLengths.txt", argv[6], argv[2]);
    return 0;
}
