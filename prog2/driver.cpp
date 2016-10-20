#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "DP.h"
#include "Util.h"
#include "FrontEnd.h"
using namespace std;

int main(int argc, char* argv[]){
	
	FrontEnd fe(argv[1]);

	ifstream infile("clauses");
	
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
	   
	    clauseSet.push_back(thisLine);
	}
	if (clauseSet.back()[0]!=0){
		cout<<"input file not terminated with 0 -- exiting program"<<endl;
		exit(1);
	}
	clauseSet.pop_back();	


	DP dp(clauseSet);
	string result = dp.run_dp(0,dp.simpClauses,dp.literalValues);	
	cout<<result;
	ofstream resultOut("results");
	if (! resultOut) { std::cerr<<"Error writing to ..."<< endl; } else {
	resultOut << result;
	}
	resultOut.close();
	ifstream infile3("results");
	
	string str3;

	while (getline(infile3, str3)) {
	stringstream resultStream(str3);
	vector<Atom> results;
	int atomInd;
	string TF;
	resultStream>>atomInd;
	resultStream>>TF;
	if (TF=="T"){
		results.push_back(fe.atoms[atomInd-1]);
		cout<<"TRUE atom: "<<fe.atoms[atomInd-1].name<<endl;
	}

	}
}