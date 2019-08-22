//
// Created by codebat on 22/08/19.
//
#include "Parser.h"

bool isPunctuation(string basicString);

using namespace std;
void Parser::parseFolder(string folder){
    DIR* dirp = opendir(folder.c_str());
    struct dirent * dp;
    while((dp = readdir(dirp)) != NULL){
        string fileName(dp->d_name);
        parseFile(fileName); //do remember to close
    }
    closedir(dirp);
    avgDocumentLength =(double) (documentLengthSum / numDocuments);
    calculateWeights();
    writeIndexToDocumentInfo("indexToDocument.txt");
    writeAvgAndNumDocuments("avgAndNumDocuments.txt");
    writeInvertedIndex("invertedIndex.txt");
};

void Parser::parseFile(string fileName){
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        while(getline(file, line)){
            if(line.substr(0,5).compare(beginDocTag) == 0){
                int docLength =0;
                vector<string> processedTextHead;
                vector<string> processedTextBody;
                while(getline(file, line)){
                    if(line.substr(0,7).compare(docNoTag)){
                        //update doc number
                        line = line.substr(7);
                        line.replace(line.find(docNoTagEnd), line.length(),"");
                        line.substr(1,line.length()-1);
                        indexToDocumentInfo.push_back(line);
                        documentToIndexTable.insert(pair<string,int>(line, numDocuments));
                        break;
                    }
                }
                while(getline(file, line)){
                    if(line.substr(0,6).compare(headTag)){
                        //parseHead
                        line = line.substr(6);
                        line.replace(line.find(headTagEnd), line.length(),"");
                        docLength += line.length();
                        processedTextHead = parseNonTaggedTextFromString(line);
                        break;
                    }
                }
                while(getline(file, line)){
                    if(line.substr(0,6).compare(textTag)){
                        //parseText
                        line = line.substr(6);
                        line.replace(line.find(textTagEnd), line.length(),"");
                        line = removeTags(line);
                        docLength += line.length();
                        processedTextBody = parseNonTaggedTextFromString(line);
                        break;
                    }
                }
                documentLengthVec.push_back(docLength);
                documentLengthSum += docLength;
                processedTextHead.insert(processedTextHead.end(),processedTextBody.begin(),processedTextBody.end());
                updateInvertedIndex(getIndexedText(processedTextHead));
                numDocuments++;
            }
        }
        file.close();
    }else{
        cout << "Unable to open file"<<endl;
    }
};

void Parser::updateInvertedIndex(map<string, int>  wordFreqDoc){
    for(map<string, int>::iterator it= wordFreqDoc.begin(); it != wordFreqDoc.end(); it++){
        map<string, TermParameters>::iterator indexInverted = invertedIndex.find(it->first);
        if(indexInverted!= invertedIndex.end()){
            indexInverted->second.dfi = indexInverted->second.dfi + 1;
            indexInverted->second.documentIndexFrequency.insert({numDocuments, it->second});
        }else{
            TermParameters * temp = new TermParameters(numDocuments, it->second);
            invertedIndex.insert({it->first, *temp});
        }
    }
}

map<string, int>  Parser::getIndexedText(vector<string> vec){
    map<string, int> freqMap;
    map<string, int>::iterator mapIt;
    for(int i=0;i<vec.size();i++){
        string temp = vec[i];
        if(freqMap.find(temp)== freqMap.end()){
            freqMap[temp] = 1;
        }else{
            freqMap[temp] = freqMap[temp] + 1;
        }
    }
    return freqMap;
}
string Parser::removeTags(string temptext){
    string text(temptext);
    size_t pos =0;
    size_t nextPos =0;
    while((pos==text.find("<")) != string::npos){
        nextPos = text.find(">");
        text.replace(pos,nextPos+1,"");
    }
    return text;
}

vector<string> Parser::split(string text){
    size_t pos =0;
    string token;
    vector<string> ans;
    while((pos==text.find(" ")) != string::npos){
        token = text.substr(0,pos);
        transform(token.begin(), token.end(), token.begin(),
                          [](unsigned char c){ return tolower(c); });
        if(!isStopWord(token) && !isPunctuation(token) && token.length()>0){
            ans.push_back(token);
        }
        text.erase(0, pos + 1);
    }
    return ans;
}

bool isPunctuation(string basicString) {
    return any_of(basicString.begin(), basicString.end(), ::ispunct);
}

bool Parser::isStopWord(string basicString) {
    return find(stopWord->begin(),stopWord->end(),basicString) == stopWord->end();
};
vector<string> Parser::parseNonTaggedTextFromString(string text){
    string temptext(text);
    vector<string> splittedVec = split(temptext);
    return splittedVec;
}

void Parser::calculateWeights() {
    double ans;
    for(map<string, TermParameters>::iterator it = invertedIndex.begin(); it!= invertedIndex.end(); it++){
        ans = log((numDocuments*1.0 - it->second.dfi + 0.5)/(it->second.dfi + 0.5));
        it->second.weight = ans;
    }
}

void Parser::writeIndexToDocumentInfo(string fileName) {
    ofstream file (fileName);
    if(file.is_open()){
        for(auto it = indexToDocumentInfo.begin(); it!= indexToDocumentInfo.end();it++){
            file << *it << "\n";
        }
        file.close();
    }

}

void Parser::writeInvertedIndex(string fileName) {
    ofstream file (fileName);
    if(file.is_open()){
        for(auto it = invertedIndex.begin(); it != invertedIndex.end(); it++){
            file << it->first << "\n";
            file << it->second.weight << "\n";
            file << it->second.dfi << "\n";
            for(auto itTermParams = it->second.documentIndexFrequency.begin();itTermParams != it->second.documentIndexFrequency.end();it++){
                file << itTermParams->first << "\n";
                file << itTermParams->second << "\n";
            }
            if(it->second.documentIndexFrequency.size()-it->second.dfi != 0){
                cerr << "error" << endl;
            }
        }
        file.close();
    }
}

void Parser::writeAvgAndNumDocuments(string fileName) {
    ofstream file (fileName);
    if(file.is_open()){
        file << avgDocumentLength << "\n";
        file << numDocuments << "\n";
        file.close();
    }
}



