//
// Created by codebat on 23/08/19.
//

#include "Query.h"

void
Query::searchFile(string fileName, string indextoDocumentFile, string invertedIndexFile, string avgNumDocumentsLengthFile,
                  string documentLengthsFile, string outputfile, string indexFileStructure) {
    ifstream file (fileName);
    string line;
    outputFileName = outputfile;
    outputFile.open(outputFileName);
    if(file.is_open() && outputFile.is_open()){
        //read encoded data first
        cout << "reading avg"<<endl;
        readAvgAndNumDocuments(indexFileStructure + avgNumDocumentsLengthFile);
        cout << "reading index"<<endl;
        readIndexToDocumentInfo(indexFileStructure+indextoDocumentFile);
        cout << "reading length"<<endl;
        readDocumentLengths(indexFileStructure+documentLengthsFile);
        cout << "reading index"<<endl;
        readInvertedIndex(indexFileStructure+invertedIndexFile);

        while(getline(file,line)){
            if(line.substr(0,topTag.length()).compare(topTag)==0){
                int topicNumber;
                vector<string> processedDomain;
                vector<string> processedTitle;
                vector<string> processedDescription;
                vector<string> processedSummary;
                vector<string> processedNarrative;
                vector<string> processedConcepts;
                //num
                while(getline(file,line)){
                    if(line.substr(0,numTag.length()).compare(numTag)==0){
                        line = line.substr(numTag.length());
                        line.replace(line.find(numTagTrashText),numTagTrashText.length(),"");
                        topicNumber = stoi(line);
                        break;
                    }
                }
                cout << "Finding answer for topic number "<<topicNumber<<endl;
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
                        break;
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
                        break;
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
                        break;
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
                        break;
                    }
                }
                vector<string> processed;
                for(int wtc=0;wtc<domainWeight;wtc++){
                    processed.insert(processed.end(),processedDomain.begin(),processedDomain.end());
                }
                for(int wtc=0;wtc<titleWeight;wtc++){

                    processed.insert(processed.end(),processedTitle.begin(),processedTitle.end());
                }

                for(int wtc=0;wtc<descriptionWeight;wtc++){

                    processed.insert(processed.end(),processedDescription.begin(),processedDescription.end());
                }

                for(int wtc=0;wtc<summaryWeight;wtc++){

                    processed.insert(processed.end(),processedSummary.begin(),processedSummary.end());
                }

                for(int wtc=0;wtc<narrativeWeight;wtc++){

                    processed.insert(processed.end(),processedNarrative.begin(),processedNarrative.end());
                }

                for(int wtc=0;wtc<conceptsWeight;wtc++){
                    processed.insert(processed.end(),processedConcepts.begin(),processedConcepts.end());

                }

                

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
                writeOutput(answer, topicNumber);


            }
        }
        file.close();
        outputFile.close();
    }else{
        cerr << "Unable to open file"<<endl;
    }
}
string Query::trimPunctAndInteger(string str) {
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
bool Query::isInteger(string basicString) {
    string::const_iterator it = basicString.begin();
    while(it!=basicString.end() && isdigit(*it)){
        it++;
    }
    return !basicString.empty() && it == basicString.end();
}
bool Query::isStopWord(string basicString) {
    for(int i=0; i< numStopWords;i++){
        if(basicString.compare(stopWord[i]) == 0){
            return true;
        }
    }
    return false;
};

vector<string> Query::split(string text){
    size_t pos =0;
    string token;
    vector<string> ans;
    while((pos=text.find(" ")) != string::npos || text.size()>0){
        token = text.substr(0,pos);
        transform(token.begin(), token.end(), token.begin(),
                  [](unsigned char c){ return tolower(c); });
        token = trimPunctAndInteger(token);
        if(!isStopWord(token)  && token.length()>0){
            ans.push_back(token);
        }
        if(pos==string::npos){
            break;
        }
        text.erase(0, pos + 1);
    }
    return ans;
}
vector<string> Query::parseNonTaggedTextFromString(string text) {
    string temptext(text);
    vector<string> splittedVec = split(temptext);
    return splittedVec;
}

map<string, int> Query::getIndexedText(vector<string> vec) {
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

map<int,double> Query::getScoreForTopic(map<string, int> mymap) {
    map<int, double> documentToScore;
    for(auto queryTermIterator = mymap.begin(); queryTermIterator != mymap.end(); queryTermIterator++){
        string term = queryTermIterator->first;
        auto parameters = invertedIndex.find(term);
        if(parameters!=invertedIndex.end()){
            int qfi = queryTermIterator->second;
            TermParameters termParameters= parameters->second;
            double weight = termParameters.weight;
            for(auto docIterator = termParameters.documentIndexFrequency.begin()
                    ;docIterator != termParameters.documentIndexFrequency.end();
                    docIterator++){
                int docNo = docIterator->first;
                int tfi = docIterator->second;
                int scoreTermDoc = getScoreForTermForDocument(qfi,docNo,weight,tfi);
                if(documentToScore.find(docNo)!=documentToScore.end()){
                    documentToScore[docNo] = documentToScore[docNo] + scoreTermDoc;
                }else{
                    documentToScore[docNo] = scoreTermDoc;
                }
            }
        }else{

        }
    }
    return documentToScore;
}

double Query::getScoreForTermForDocument(int qfi, int docNo, int weight, int tfi) {
    double documentLength = documentLengthVec[docNo] * 1.0;
    return ((qfi)* (tfi*(1+k1)) * (weight))/(k1 * ((1-b) + b * (documentLength/avgDocumentLength) )+tfi);
}

vector<pair<int, double>> Query::sort_index(map<int,double> documentToScore) {
    int numValuesAdded=0;
    vector<pair<int,double>> ans;
    while(numValuesAdded<maxRanking && documentToScore.size()!=0){
        auto x = max_element(documentToScore.begin(),documentToScore.end(),
                             [](const pair<int,double>& p1, const pair<int,double>& p2){
                                 return p1.second < p2.second;
                             });
        ans.push_back(make_pair(x->first,x->second));
        numValuesAdded++;
        documentToScore.erase(x);
    }
    return ans;

}

vector<pair<string, double>> Query::giveRankForTopic(vector<pair<int, double>> vec) {
    vector<pair<string, double>> ans;
    for(auto it = vec.begin(); it!= vec.end();it++){
        ans.push_back(make_pair(indexToDocumentInfo[it->first], it->second));
    }
    return ans;
}

void Query::readIndexToDocumentInfo(string fileName) {
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        while(getline(file,line)){
            indexToDocumentInfo.push_back(line);
        }
        file.close();
    }else{
        cerr << "Unable to open file";
    }
}

void Query::readInvertedIndex(string fileName) {
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        while(getline(file,line)){
            string term = line;
            getline(file,line);
            double weight = stod(line);
            getline(file,line);
            int dfi = stoi(line);
            TermParameters * termParameters = new TermParameters(weight,dfi);
            for(int i = 0 ; i<dfi;i++){
                getline(file,line);
                int docIndex = stoi(line);
                getline(file,line);
                int tfi = stoi(line);
                termParameters->documentIndexFrequency[docIndex] = tfi;
            }
            invertedIndex.insert({term,*termParameters});
        }
        file.close();
    }else{
        cerr << "Unable to open file";
    }
}

void Query::readAvgAndNumDocuments(string fileName) {
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        getline(file,line);
        avgDocumentLength = stod(line);
        getline(file,line);
        numDocuments = stoi(line);
        file.close();
    }else{
        cerr << "Unable to open file";
    }
}

void Query::readDocumentLengths(string fileName) {
    ifstream file (fileName);
    string line;
    if(file.is_open()){
        while(getline(file,line)){
            documentLengthVec.push_back(stoi(line));
        }
        file.close();
    }else{
        cerr << "Unable to open file";
    }
}

void Query::writeOutput(vector<pair<string, double>> vec, int number) {
//    if(outputFile.is_open()){
        int count =1;
        for(auto it = vec.begin(); it!= vec.end(); it++){
            outputFile<< number << " 0 " << it->first << " " <<  count << " "<< it->second << " p\n";
            count ++;
        }
//        outputFile.close();
//    }else{
//        cerr << "Unable to open file"<<endl;
//    }
}
