#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "DP.h"
#include "Util.h"
using namespace std;

int main(int argc, char* argv[]){
	
	ifstream infile(argv[1]);
	string str;

	vector<vector<int>> clauseSet;

	int count = 0;
	cout<<"infile: "<<argv[1]<<endl;
	
	while (getline(infile, str)) {
		vector<int> thisLine;
		stringstream ss(str);
		int i;
	    while (ss >> i)
	    {

	    thisLine.push_back(i);
	        if (ss.peek() == ',')
	            ss.ignore();
	    }
	    cout<<"count: "<<++count<<endl;
	    for (int i=0;i<thisLine.size();++i){
	    	cout<<thisLine[i]<<" ";
	    }
	    cout<<endl;
	    clauseSet.push_back(thisLine);
	    cout<<"ello"<<endl;
	}
	if (clauseSet.back()[0]!=0){
		cout<<"input file not terminated with 0 -- exiting program"<<endl;
		exit(1);
	}
	clauseSet.pop_back();	

	ifstream infile2(argv[2]);
	string str2;
	getline(infile2, str2);
	stringstream ss1(str2);
	string s;
	vector<string> nodeList;
	while (ss1 >> s)
	    {
	    nodeList.push_back(s);
	        if (ss1.peek() == ',')
	            ss1.ignore();
	}
	cout<<"node list size: "<<nodeList.size()<<endl;
	getline(infile2, str2);
	stringstream ss2(str2);
	
	vector<string> treasureList;
	while (ss2 >> s)
	    {
	    treasureList.push_back(s);
	        if (ss1.peek() == ',')
	            ss1.ignore();
	}
	getline(infile2, str2);
	int maxSteps = atoi(str2.c_str());
	vector<vector<string>> edges; //[currentNode][adjacentNode]
	vector<vector<string>> treasures;
	vector<vector<string>> tolls;
	vector<Atom> atoms;
	vector<vector<Atom>> atAtoms; //[time][node]
	vector<vector<Atom>> hasAtoms; //[time][treasure]
	vector<vector<Atom>> availAtoms; //[time][treasure]

	count = 0;


	while(getline(infile2,str2)){
		stringstream ss3(str2);
		string node;
		ss3>>node;
		string i;
		cout<<"node: "<<node<<endl;
		vector<string> adjacentNodes;
		vector<string> nodeTreasures;
		vector<string> nodeTolls;
		ss3>>i;

		while (ss3 >> i)
	    {
	    if (i=="TOLLS"){
	    	break;
	    }
	    nodeTreasures.push_back(i);
		}
		treasures.push_back(nodeTreasures);
		
		while (ss3 >> i)
	    {
	    if (i=="NEXT"){
	    	break;
	    }
	    nodeTolls.push_back(i);
		}
		tolls.push_back(nodeTolls);

		while (ss3 >> i)
	    {
	    adjacentNodes.push_back(i);
	        
		}
		edges.push_back(adjacentNodes);
	}
	cout<<"edge atoms:"<<endl;
	for (int i=0;i<edges.size();++i){
		for (int j=0;j<edges[i].size();++j){
			cout<<edges[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"node treasures:"<<endl;
	for (int i=0;i<treasures.size();++i){
		for (int j=0;j<treasures[i].size();++j){
			cout<<treasures[i][j]<<" ";
		}
		cout<<endl;
	}
	cout<<"node tolls:"<<endl;
	for (int i=0;i<tolls.size();++i){
		for (int j=0;j<tolls[i].size();++j){
			cout<<tolls[i][j]<<" ";
		}
		cout<<endl;
	}

	for (int i=0;i<=maxSteps;++i){
		vector<Atom> nodes;
		for (int j=0;j<nodeList.size();++j){
			string atomString = "At("+nodeList[j]+","+to_string(i)+")";
			Atom thisAtom(i,"",nodeList[j],"At",atomString,++count);
			nodes.push_back(thisAtom);
			atoms.push_back(thisAtom);

		}
		atAtoms.push_back(nodes);
	}
	for (int i=0;i<=maxSteps;++i){
		vector<Atom> hasTreasures;
		vector<Atom> availTreasures;
		for (int j=0;j<treasureList.size();++j){
			string atomString = "Has("+treasureList[j]+","+to_string(i)+")";
			Atom thisAtom(i,treasureList[j],"","Has",atomString,++count);
			hasTreasures.push_back(thisAtom);
			atoms.push_back(thisAtom);
			atomString = "Available("+treasureList[j]+","+to_string(i)+")";
			Atom thisAtom2(i,treasureList[j],"","Available",atomString,++count);
			availTreasures.push_back(thisAtom2);
			atoms.push_back(thisAtom2);
		}
		hasAtoms.push_back(hasTreasures);
		availAtoms.push_back(availTreasures);
	}

	for (int i=0;i<atoms.size();++i){
		cout<<atoms[i].index<<" "<<atoms[i].name<<endl;
	}
	string clauses;
	cout<<"hello"<<endl;
	cout<<"atAtoms.size(): "<<atAtoms.size()<<endl;
	for (int i=0;i<=maxSteps;++i){
		for (int j=0;j<atAtoms[i].size()-1;++j){
			for (int k=j+1;k<atAtoms[i].size();++k){
				//category 1
				clauses+="-"+atAtoms[i][j].name+" -"+atAtoms[i][k].name+"\n";
			}
		}
		for (int j=0;j<hasAtoms[i].size();++j){
			clauses+="-"+hasAtoms[i][j].name+" -"+availAtoms[i][j].name+"\n";
		}

	}
	//cout<<"clauses: "<<clauses<<endl;


	DP dp(clauseSet);
	string result = dp.run_dp(0,dp.simpClauses,dp.literalValues);	
	cout<<result;
}