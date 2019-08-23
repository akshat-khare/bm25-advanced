//
// Created by codebat on 23/08/19.
//

#include "Query.h"

void
Query::searchFile(string file, string indextoDocumentFile, string invertedIndexFile, string avgNumDocumentsLength,
                  string documentLengthsFile) {

}

vector<string> Query::parseNonTaggedTextFromString(string text) {
    return vector<string>();
}

map<string, int> Query::getIndexedText(vector<string> vec) {
    return map<string, int>();
}

vector<pair<double, int>> Query::getScoreForTopic(map<string, int> vec) {
    return vector<pair<double, int>>();
}

double Query::getScoreForTermForDocument(string term, int qfi, int docNo) {
    return 0;
}

vector<pair<double, int>> Query::sort_index(vector<pair<double, int>> vec) {
    return vector<pair<double, int>>();
}

vector<pair<string, double>> Query::giveRankForTopic(vector<pair<double, int>> vec) {
    return vector<pair<string, double>>();
}

void Query::readIndexToDocumentInfo(string fileName) {

}

void Query::readInvertedIndex(string fileName) {

}

void Query::readAvgAndNumDocuments(string fileName) {

}

void Query::readDocumentLengths(string fileName) {

}
