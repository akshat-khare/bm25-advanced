//
// Created by codebat on 23/08/19.
//

#ifndef BM25_QUERY_H
#define BM25_QUERY_H

#include <stdio.h>
#include <string>
#include <map>
#include <set>
#include <tuple>
#include <fstream>
#include <iostream>
#include <vector>
#include <dirent.h>
#include <algorithm>
#include <math.h>
#include "TermParameters.h"

using namespace std;

class Query {
public:
    Query(){

    };
    double k1 = 0.0;
    double b = 0.0;
    double avgDocumentLength; //read 1
    vector<string> indexToDocumentInfo; //read
    vector<int> documentLengthVec; //read
    map<string, TermParameters> invertedIndex; //read
    int numDocuments; //read 1
    void searchFile(string fileName, string indextoDocumentFile, string invertedIndexFile, string avgNumDocumentsLength, string documentLengthsFile);
    string outputFileName = "trecOut.txt";
    ofstream outputFile;

    vector<string> parseNonTaggedTextFromString(string text);
    map<string, int> getIndexedText(vector<string> vec);
    map<int,double> getScoreForTopic(map<string,int> vec);
    double getScoreForTermForDocument(int qfi, int docNo, int weight, int tfi);
    vector<pair<int, double>> sort_index(map<int,double>);
    vector<pair<string, double>> giveRankForTopic(vector<pair<int,double>> vec);
    vector<string> split(string text);
    bool isStopWord(string basicString);
    string trimPunctAndInteger(string str);
    bool isInteger(string basicString);

    void readIndexToDocumentInfo(string fileName);
    void readInvertedIndex(string fileName);
    void readAvgAndNumDocuments(string fileName);
    void readDocumentLengths(string fileName);

    string topTag = "<top>";
    string numTag = "<num>";
    string numTagTrashText = " Number:  ";
    string domTag = "<dom>";
    string domainTrashText = " Domain:  ";
    string titleTag = "<title>";
    string domainTopicText = " Topic:  ";
    string descTag = "<desc>";
    string narrativeTag = "<narr>";
    string summaryTag = "<smry>";
    string conceptsTag = "<con>";
    string factorsTag = "<fac>";
    int maxRanking = 50;
    int numStopWords = 127;
    string stopWord[127] = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};


    void writeOutput(vector<pair<string, double>> vector, int number);
};


#endif //BM25_QUERY_H
