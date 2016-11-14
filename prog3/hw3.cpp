#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include <errno.h>
#include "Reviewer.h"
#include "Search.h"

using namespace std;
int main (int argc, char* argv[]){
	
	ifstream infile(argv[1]);
	
	string str;
	int rCount;
	double S,F,p;


	getline(infile,str);
	istringstream ss(str);
	ss>>rCount>>S>>F>>p;
	
	//cout<<"rCount: "<<rCount<<", S: "<<S<<", F: "<<F<<", p: "<<p<<endl;
	vector<Reviewer> reviewers;

	while(getline(infile,str)){
		istringstream ss2(str);
		double cost,RT,RF;
		ss2>>cost >>RT>>RF;
		//cout<<"cost: "<<cost<<", RT: "<<RT<<", RF: "<<RF<<endl;
		Reviewer r(cost,RT,RF);
		reviewers.push_back(r);
	}
	double bestMove = 0;
	double maxUtil=0;
	vector<int> states;
	for (int i=0;i<reviewers.size();i++){
		states.push_back(0);	
	}
	for (int i=0;i<reviewers.size()+1;i++){
		Search search(states,reviewers,p,S,F,i);
		cout<<"expected value: "<<search.maxUtil<<endl;
		//cout<<"bestmove: "<<search.bestMove<<endl;
		if (search.bestMove==-1){
		cout<<"reject"<<endl;
		break;
		}
		if (search.bestMove == 0){
		cout<<"publish"<<endl;
		break;
		}
		if (search.bestMove>0)
		cout<<"consult reviewer "<<search.bestMove<<": ";

		string response;
		cin>>response;
		if (response=="yes")
			states[i]=search.bestMove;
		else
			states[i]=-search.bestMove;
		//cout<<response<<endl;
		
	}
	

	
	
	
		

	
}