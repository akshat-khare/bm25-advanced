//
// Created by codebat on 22/08/19.
//

#ifndef BM25_PARSER_H
#define BM25_PARSER_H
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
using namespace std;
class TermParameters{
public:
    TermParameters(int documentIndex, int frequency){
        dfi = 1;
        documentIndexFrequency[documentIndex] = frequency;
    }
    int getDfi();
    double getWeight();
    set<tuple<int, int>> getDocumentIndexFrequency();
    double weight;
    int dfi;
    map<int,int> documentIndexFrequency;
};
class Parser{
public:
    Parser(){

    };
    int numStopWords = 127;
    string stopWord[127] = {"i", "me", "my", "myself", "we", "our", "ours", "ourselves", "you", "your", "yours", "yourself", "yourselves", "he", "him", "his", "himself", "she", "her", "hers", "herself", "it", "its", "itself", "they", "them", "their", "theirs", "themselves", "what", "which", "who", "whom", "this", "that", "these", "those", "am", "is", "are", "was", "were", "be", "been", "being", "have", "has", "had", "having", "do", "does", "did", "doing", "a", "an", "the", "and", "but", "if", "or", "because", "as", "until", "while", "of", "at", "by", "for", "with", "about", "against", "between", "into", "through", "during", "before", "after", "above", "below", "to", "from", "up", "down", "in", "out", "on", "off", "over", "under", "again", "further", "then", "once", "here", "there", "when", "where", "why", "how", "all", "any", "both", "each", "few", "more", "most", "other", "some", "such", "no", "nor", "not", "only", "own", "same", "so", "than", "too", "very", "s", "t", "can", "will", "just", "don", "should", "now"};
    string beginDocTag = "<DOC>";
    string docNoTag = "<DOCNO>";
    string docNoTagEnd = "</DOCNO>";
    string headTag = "<HEAD>";
    string headTagEnd = "</HEAD>";
    string textTag = "<TEXT>";
    string textTagEnd = "</TEXT>";
public:
    string datasetFolder;
    double avgDocumentLength; //write 1
    map<string, int> documentToIndexTable;
    vector<string> indexToDocumentInfo; //write
    vector<int> documentLengthVec; //write
    map<string, TermParameters> invertedIndex; //write
    long long documentLengthSum;
    int numDocuments = 0; //write 1
    bool isStopWord(string basicString);
    void parseFile(string file);
    void parseFolder(string folder);
    vector<string> parseNonTaggedTextFromString(string text);
    string removeTags(string text); // TODO: CHANGE
    map<string, int> getIndexedText(vector<string>);
    void updateInvertedIndex(map<string, int>  wordFreqDoc);
    void writeIndexToDocumentInfo(string fileName);
    void writeInvertedIndex(string fileName);
    void writeAvgAndNumDocuments(string fileName);
    vector<string> split(string text);


    void calculateWeights();

    string trimPunct(string basicString);
};

#endif //BM25_PARSER_H
