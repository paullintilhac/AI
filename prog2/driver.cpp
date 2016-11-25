#include <stdio.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "DP.h"
#include "Util.h"
#include "FrontEnd.h"
#include "Clauses.h"
using namespace std;

int main(int argc, char* argv[]){
	//set up front end to read from maze file
	FrontEnd fe(argv[1]);


	ifstream infile("clauses");
	string str;

	//digest clauses into 2-D vector
	vector<vector<int> > clauseSet;
	int count=0;

	while (getline(infile, str)) {

		vector<int> thisLine;
		stringstream ss(str);
		int i;

	    while (ss >> i)
	    {

	    thisLine.push_back(i);
	       
	    }
	   
	    clauseSet.push_back(thisLine);
	}




	if (clauseSet.back()[0]!=0){
		cout<<"input file not terminated with 0 -- exiting program"<<endl;
		exit(1);
	}

	clauseSet.pop_back();	

	//construct DPLL object
	DP dp(clauseSet,fe.atoms,false);
	//construct clauses object
	Clauses clauses(dp.simpClauses,dp.literalValues);
	//run DPLL
	Clauses result = dp.run_dp(0,clauses,0);	
	//cout<<"result status: "<<result.status<<endl;

	result.getReturnString();

	//output return of DPLL to file
	ofstream resultOut("results");
	if (! resultOut) { std::cerr<<"Error writing to ..."<< endl; } else {
	resultOut << result.returnString;
	}

	resultOut.close();

	//open file back up to produce back end
	ifstream infile3("results");
	
	string str3;
	int failCount=0;
	//cout<<"SOLUTION TO THE GAME (see At(.,.) atoms for the actual path): "<<endl;
	while (getline(infile3, str3)) {

	stringstream resultStream(str3);
	vector<Atom> results;
	int atomInd;
	string TF;
	resultStream>>atomInd;
	
	if (failCount++==0 &&atomInd==0){
		cout<<"no solution found"<<endl;
	}
	if (atomInd == 0){
		break;
	}
	resultStream>>TF;
	if (TF=="T"){
		results.push_back(fe.atoms[atomInd-1]);
		cout<<fe.atoms[atomInd-1].name<<endl;
	}
	}
	if (false){
	cout<<"printing set of clauses, size: "<<clauseSet.size()<<endl;
	for (int i=0;i<clauseSet.size();++i){
		string clauseString="";
		for (int l=0;l<clauseSet[i].size();++l){
			if (clauseSet[i][l]<0)
			clauseString+="~"+fe.atoms[abs(clauseSet[i][l])-1].name+" ";
			if (clauseSet[i][l]>0)
				clauseString+=fe.atoms[abs(clauseSet[i][l])-1].name+" ";
		}
		clauseString+="\n";
		cout<<clauseString;
		//cout<<fe.atoms[i].name<<endl;
	}
}
}
