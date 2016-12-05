#ifndef DIGESTER_H
#define DIGESTER_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <math.h>
#include "Bio.h"
#include "Model.h"
using namespace std;


class Digester{
public:
	char* infileName;
	char* stopfileName;
	vector<Bio> bios;
	vector<string> wordBag;
	vector<string> catBag;
	vector<vector<double> > freqs;
	vector<vector<double> > probs;
	vector<vector<double> > logProbs;
	vector<vector<double> > counts;
	vector<double> probVec;
	vector<double> freqVec;
	vector<double> logProbVec;
	Digester(char* in, char* stop):infileName(in),stopfileName(stop){
		ifstream infile(in);
		ifstream stopfile(stop);
		string temp;
		vector<string> stopWords;

		//get stop words
		while(getline(stopfile,temp)){
			string i;
			stringstream ss(temp);
			while (ss >> i)
		    {
		    
		    stopWords.push_back(i);
		       
		    }
		}
		string str;
		//digest clauses into 2-D vector
		//vector<vector<int> > clauseSet;
		int count=0;
		int newBio = 1;
		string cat,name;
		vector<string> words;
		int index = 0;

		//get array of Bio objects
		while (getline(infile, str)) {
			//cout<<"newBio: "<<newBio<<endl;
			if (str.length() == 1){
				bool success = getline(infile,str);
				if (!success)
					break;
				//cout<<"success? "<<success<<endl;
				//cout<<"final str: "<<str<<endl;
				if (str.length()>1){//check for multiple blank lines
				newBio = 1;
				
				index++;
			}
				//cout<<"null line"<<endl;
			}

			if (newBio==1){
				name = str;
				name.erase(remove(name.begin(), name.end(), '\r'), name.end());
				if (name[name.length()-1]==' '){
					name = name.substr(0,name.length()-1);
				}


				words.clear();
				newBio = 2;
				continue;
			} 
			if (newBio == 2){
				cat = str;
				cat.erase(remove(cat.begin(), cat.end(), '\r'), cat.end());
				if (cat[cat.length()-1]==' '){
					cat = cat.substr(0,cat.length()-1);
				}

				//cout<<"catBag.size(): "<<catBag.size()<<endl;
				bool seenCat = false;
			   	for (int j=0;j<catBag.size();j++){
			   		//cout<<cat<<" compareCat: "<<catBag[j]<<"--"<<endl;
			   		if (cat==catBag[j]){
			   			seenCat = true;
			   			break;
			   		}
			   	}
			   	if (!seenCat){
			   		//cout<<"new category: -"<<cat<<"-"<<endl;
			   		catBag.push_back(cat);
			   	}
			   	newBio = 0;
			   	continue;
			 }
			string i;
			stringstream ss(str);

		    while (ss >> i)
		    {
		    if (i.length()<=2)
		    	continue;
		    bool stop = false;
		    for (int j=0;j<stopWords.size();j++){
		    	if (i == stopWords[j]){
		    		//cout<<"stop word: "<<stopWords[j]<<endl;
		    		stop = true;
		    		break;
		    	}
		    }
		    if (stop)
		    	continue;
		    i.erase(remove(i.begin(), i.end(), ','), i.end());
		   	i.erase(remove(i.begin(), i.end(), '.'), i.end());
			i.erase(remove(i.begin(), i.end(), '\r'), i.end());
			transform(i.begin(), i.end(), i.begin(), ::tolower);
			if (i[i.length()-1]==' '){
					i = i.substr(0,i.length()-1);
				}

		   	bool seenWord = false;
		   	for (int j=0;j<wordBag.size();j++){
		   		if (i==wordBag[j]&& j!=wordBag.size()-1){
		   			seenWord = true;
		   			break;
		   		}
		   	}
		   	if (!seenWord){
		   		wordBag.push_back(i);
		   	}

		    words.push_back(i);
		       
		    }
		    Bio thisBio(words,index,cat,name);
			bios.push_back(thisBio);
		}
//cout<<"word bag";
	// for (int i=0;i<wordBag.size();i++){
	// 	cout<<"_"<<wordBag[i]<<"_  "<<endl;;
	// }
	// cout<<endl;
	// for (int i=0;i<catBag.size();i++){
	// 	cout<<catBag[i]<<"  ";
	// 	cout.flush();
	// 	for (int j=0;j<wordBag.size();j++){
	// 		cout<<probs[i][j]<<" ";
	// 		cout.flush();
	// 	}
	// 	cout<<endl;
	// }
	// 
		//cout<<"BIOS SIZE: "<<bios.size()<<endl;
 }
 	Model train(double eps){
 		//get conditional and absolute probabilities

		for (int i=0;i<catBag.size();i++){
			vector<double> probRow;
			vector<double> freqRow;
			vector<double> logProbRow;
			vector<double> countRow;
			for (int j=0;j<wordBag.size();j++){
				double count=0;
				double totalCount=0;
				for (int k=0;k<bios.size();k++){
					//cout<<"catBag[i]: "<<catBag[i]<<", wordBag[j]: "<<wordBag[j]<<", bios category: "<<bios[k].category<<endl;
					if (bios[k].category!=catBag[i] )
						continue;
					//if (catBag[i] == "Government" &&wordBag[j]=="politician")
					totalCount++;
					bool hasWord=false;
					//cout<<"i: "<<i<<", j: "<<j<<", k: "<<k<<endl;
					for (int l=0;l<bios[k].words.size();l++){
						if (bios[k].words[l]==wordBag[j]){
							hasWord=true;
							break;
						}
					}
					if (hasWord)
						count++;
				}
				countRow.push_back(count);
				freqRow.push_back(count/totalCount);
				probRow.push_back((count/totalCount+eps)/(1+2*eps));
				logProbRow.push_back(-log2((count/totalCount+eps)/(1+2*eps)));
			}
			counts.push_back(countRow);
			freqs.push_back(freqRow);
			probs.push_back(probRow);
			logProbs.push_back(logProbRow);
		}

		for (int i=0;i<catBag.size();i++){

			int count = 0;
			for (int j=0;j<bios.size();j++){
				//cout<<"i: "<<i<<", j: "<<j<<endl;
				if (bios[j].category == catBag[i])
					count++;
			}
			freqVec.push_back((double)count/bios.size());
			probVec.push_back(((double)count/bios.size()+eps)/(1+catBag.size()*eps));
			logProbVec.push_back(-log2(((double)count/bios.size()+eps)/(1+catBag.size()*eps)));
			//cout<<"count/bios.size(): "<<((double)count/bios.size())<<", logProbVec: "<<logProbVec[i]<<endl;
		}
	//print model specification
	if (true){
	cout<<"printing model specification"<<endl;
	cout<<"               ";
	for (int i=0;i<catBag.size();i++){
		cout<<setw(15)<<catBag[i];
	}
	cout<<endl;
	for (int i=0;i<wordBag.size();i++){
		cout<<setw(15)<<wordBag[i];
		for (int j=0;j<catBag.size();j++){
			cout<<setw(15)<<logProbs[j][i];
		}
		cout<<endl;
	}

	}
	Model model(logProbs,logProbVec,catBag,wordBag);
 	return(model);

 	};
 
	
};
#endif