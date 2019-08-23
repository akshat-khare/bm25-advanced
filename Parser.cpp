//
// Created by codebat on 22/08/19.
//
#include "Parser.h"

bool isPunctuation(string basicString);

using namespace std;
// #define WINDOWS  /* uncomment this line to use it for windows.*/
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
std::string GetCurrentWorkingDir( void ) {
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    return current_working_dir;
}
void Parser::parseFolder(string folder){
    cout << GetCurrentWorkingDir()<<endl;
    DIR* dirp = opendir(folder.c_str());
    struct dirent * dp;
    vector<string> files;
    while((dp = readdir(dirp)) != NULL){
        string fileName(dp->d_name);
        if(fileName.compare(".")==0 || fileName.compare("..")==0){
            continue;
        }
        files.push_back(fileName);
        cout << "Parsing "<< fileName << " "<< numDocuments << " yet which is number "<< (files.size()+1)<< endl;
        parseFile(folder +"/"+ fileName); //do remember to close
    }
//    for(int i =0;i< files.size();i++){
//        cout << files[i] << endl;
//    }
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
                    if(line.substr(0,7).compare(docNoTag)==0){
                        //update doc number
                        line = line.substr(7);
                        line.replace(line.find(docNoTagEnd), line.length(),"");
                        line = line.substr(1,line.length()-2);
                        indexToDocumentInfo.push_back(line);
                        documentToIndexTable.insert(pair<string,int>(line, numDocuments));
                        break;
                    }
                }
                while(getline(file, line)){
                    if(line.substr(0,6).compare(headTag)==0){
                        //parseHead
                        line = line.substr(6);
                        size_t endHeadPos = line.find(headTagEnd);
                        while(endHeadPos==string::npos){
                            string nextLine;
                            getline(file, nextLine);
                            line = line + " " + nextLine;
                            endHeadPos = line.find(headTagEnd);
                        }
                        line.replace(line.find(headTagEnd), line.length(),"");
                        docLength += line.length();
                        processedTextHead = parseNonTaggedTextFromString(line);
                        break;
                    }
                }
                while(getline(file, line)){
                    if(line.substr(0,6).compare(textTag)==0){
                        getline(file,line);
                        //parseText
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
    while((pos=text.find("<")) != string::npos){
        nextPos = text.find(">");
        text.replace(pos,nextPos-pos+1,"");
    }
    return text;
}

vector<string> Parser::split(string text){
    size_t pos =0;
    string token;
    vector<string> ans;
    while((pos=text.find(" ")) != string::npos || text.size()>0){
        token = text.substr(0,pos);
        transform(token.begin(), token.end(), token.begin(),
                          [](unsigned char c){ return tolower(c); });
        token = trimPunctAndInteger(token);
        if(!isStopWord(token) && !isInteger(token)  && token.length()>0){
            ans.push_back(token);
        }
        if(pos==string::npos){
            break;
        }
        text.erase(0, pos + 1);
    }
    return ans;
}

bool isPunctuation(string basicString) {
    return any_of(basicString.begin(), basicString.end(), ::ispunct);
}

bool Parser::isStopWord(string basicString) {
    for(int i=0; i< numStopWords;i++){
        if(basicString.compare(stopWord[i]) == 0){
            return true;
        }
    }
    return false;
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
            for(auto itTermParams = it->second.documentIndexFrequency.begin();itTermParams != it->second.documentIndexFrequency.end();itTermParams++){
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

string Parser::trimPunctAndInteger(string str) {
    for (int i = 0, len = str.size(); i < len; i++)
    {
        if (ispunct(str[i]) || isdigit(str[i]))
        {
            str.erase(i--, 1);
            len = str.size();
        }
    }
    return str;
}

bool Parser::isInteger(string basicString) {
    string::const_iterator it = basicString.begin();
    while(it!=basicString.end() && isdigit(*it)){
        it++;
    }
    return !basicString.empty() && it == basicString.end();
}



