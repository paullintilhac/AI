#include "SearchAlgorithm.h"
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace std;

string ID::DFS(Params* p,int level, int branch,int maxLevel, int* stateVector){
	// note the level is 1-indexed
	stateVector[level-1] = branch; //update state vector


	
	//calculate value and time of current state vector
	float* eval = new float[2];
	eval = (evaluateState(stateVector));
	float totalTime = eval[0];
	float valSum = eval[1];
	//cout<<"stateVector: ";
	//for (int i=0;i<p->tCount;++i){
	//	cout<<to_string(stateVector[i]) +" ";
	//}
	//cout<<", totalTime: "<<totalTime<<", valSum: "<<valSum<<endl;;
	
	//check if goal state has been reached
	if (totalTime<=p->deadline && valSum >= p->targetVals){
		string returnString=to_string(stateVector[0]);
		for (int i=1;i<p->tCount;++i){
			returnString =returnString+" "+ to_string(stateVector[i]);
		}
		return(returnString);
	}

	//now iterate through children recursively
	for (int i=0;i<p->pCount+1;++i){
		if (level <maxLevel){
			string childReturnString = DFS(p,level+1,i,maxLevel,stateVector);
			//if the result was success, we need to exit inside loop
			if (childReturnString !="No solution"){
				return(childReturnString);
			}
		}
	}
	//entire p has been searched without goal state, meaning no solution
	return("No solution");
}
string ID::doSearch(){
		int* stateVector = new int[p->tCount];
		string returnString = "";
		//iterate over different maximum depths
		for (int j = 1;j<=p->tCount;++j){
		//reset state vector at the start of each DFS
			for (int i=0;i<p->tCount;++i){
				stateVector[i]=0;
			}
			returnString = DFS(p,0,0,j,stateVector);
			//if goal is reached, exit before full depth
			if (returnString!="No solution"){
				break;
			}
		}
	return(returnString);
}