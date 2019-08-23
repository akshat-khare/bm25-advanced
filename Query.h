//
// Created by codebat on 23/08/19.
//

#ifndef BM25_QUERY_H
#define BM25_QUERY_H

#include <string>
#include <vector>
#include "TermParameters.h"

using namespace std;

class Query {
public:
    double avgDocumentLength; //read 1
    vector<string> indexToDocumentInfo; //read
    vector<int> documentLengthVec; //read
    map<string, TermParameters> invertedIndex; //read
    int numDocuments; //read 1
    void searchFile(string file, string indextoDocumentFile, string invertedIndexFile, string avgNumDocumentsLength, string documentLengthsFile);

    vector<string> parseNonTaggedTextFromString(string text);
    map<string, int> getIndexedText(vector<string> vec);
    vector<pair<double,int>> getScoreForTopic(map<string,int> vec);
    double getScoreForTermForDocument(string term, int qfi, int docNo);
    vector<pair<double,int>> sort_index(vector<pair<double, int>> vec);
    vector<pair<string, double>> giveRankForTopic(vector<pair<double,int>> vec);


    void readIndexToDocumentInfo(string fileName);
    void readInvertedIndex(string fileName);
    void readAvgAndNumDocuments(string fileName);
    void readDocumentLengths(string fileName);

};


#endif //BM25_QUERY_H
