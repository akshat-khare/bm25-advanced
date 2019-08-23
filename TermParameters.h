//
// Created by codebat on 23/08/19.
//

#ifndef BM25_TERMPARAMETERS_H
#define BM25_TERMPARAMETERS_H
#include <map>
using namespace std;
class TermParameters{
public:
    TermParameters(int documentIndex, int frequency){
        dfi = 1;
        documentIndexFrequency[documentIndex] = frequency;
    }
    double weight;
    int dfi;
    map<int,int> documentIndexFrequency;
};
#endif //BM25_TERMPARAMETERS_H
