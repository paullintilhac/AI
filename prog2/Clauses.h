#ifndef CLAUSES_H
#define CLAUSES_H
#include <vector>
#include <string>
using namespace std;

class Clauses{
public:
	vector<vector<int> > sentences;
	vector<int> states;
	string status,returnString;
	public: Clauses(vector<vector<int> > c,vector<int> s){

		sentences = c;
		states = s;
		status = "unresolved";
		
	};
	public: void getReturnString(){
		if (status=="success"){
		returnString="";
		for (int i=0;i<states.size();++i){
		string TF = "";
		if (states[i]>0){
			TF = "T";
		} else{
			TF = "F";
		}
		returnString +=(i+1)+" "+TF+"\n";
		}
		returnString +="0\n";
		} else{
			returnString = "0\n";
		}

		this->returnString = returnString;
	};
	
};
#endif
