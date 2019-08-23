//
// Created by codebat on 23/08/19.
//

#include "Query.h"

void
Query::searchFile(string fileName, string indextoDocumentFile, string invertedIndexFile, string avgNumDocumentsLength,
                  string documentLengthsFile) {
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        //read encoded data first
        while(getline(file,line)){
            if(line.substr(0,topTag.length()).compare(topTag)==0){
                vector<string> processedDomain;
                vector<string> processedTitle;
                vector<string> processedDescription;
                vector<string> processedSummary;
                vector<string> processedNarrative;
                vector<string> processedConcepts;
                //domain
                while(getline(file,line)){
                    if(line.substr(0,domTag.length()).compare(domTag)==0){
                        line = line.substr(domTag.length());
                        line.replace(line.find(domainTrashText),domainTrashText.length(),"");
                        processedDomain = parseNonTaggedTextFromString(line);
                        break;
                    }
                }
                //topic
                while(getline(file,line)){
                    if(line.substr(0,titleTag.length()).compare(titleTag)==0){
                        line = line.substr(titleTag.length());
                        line.replace(line.find(domainTopicText),domainTopicText.length(),"");
                        processedTitle = parseNonTaggedTextFromString(line);
                        break;
                    }
                }
                //description
                while(getline(file,line)){
                    if(line.substr(0,descTag.length()).compare(descTag)==0){
                        getline(file,line);
                        getline(file,line);
                        string accLine = "";
                        while(line.length()!=0){
                            accLine += " " + line;
                            getline(file,line);
                        }
                        processedDescription = parseNonTaggedTextFromString(accLine);
                    }
                }
                //summary
                while(getline(file,line)){
                    if(line.substr(0,summaryTag.length()).compare(summaryTag)==0){
                        getline(file,line);
                        getline(file,line);
                        string accLine = "";
                        while(line.length()!=0){
                            accLine += " " + line;
                            getline(file,line);
                        }
                        processedSummary = parseNonTaggedTextFromString(accLine);
                    }
                }
                //narrative
                while(getline(file,line)){
                    if(line.substr(0,narrativeTag.length()).compare(narrativeTag)==0){
                        getline(file,line);
                        getline(file,line);
                        string accLine = "";
                        while(line.length()!=0){
                            accLine += " " + line;
                            getline(file,line);
                        }
                        processedNarrative = parseNonTaggedTextFromString(accLine);
                    }
                }
                //concepts
                while(getline(file,line)){
                    if(line.substr(0,conceptsTag.length()).compare(conceptsTag)==0){
                        getline(file,line);
                        getline(file,line);
                        string accLine = "";
                        while(line.substr(0,factorsTag.length()).compare(factorsTag)!=0){
                            accLine += " " + line;
                            getline(file,line);
                        }
                        processedConcepts = parseNonTaggedTextFromString(accLine);
                    }
                }
                vector<string> processed;
                processed.insert(processed.end(),processedDomain.begin(),processedDomain.end());
                processed.insert(processed.end(),processedTitle.begin(),processedTitle.end());
                processed.insert(processed.end(),processedDescription.begin(),processedDescription.end());
                processed.insert(processed.end(),processedSummary.begin(),processedSummary.end());
                processed.insert(processed.end(),processedNarrative.begin(),processedNarrative.end());
                processed.insert(processed.end(),processedConcepts.begin(),processedConcepts.end());

                map<string,int> queryFreq = getIndexedText(processed);
                vector<pair<string,double>> answer = giveRankForTopic(
                        sort_index(
                                getScoreForTopic(
                                        getIndexedText(
                                                processed
                                                )
                                        )
                                )
                        );


            }
        }
        file.close();
    }else{
        cerr << "Unable to open file"<<endl;
    }
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
